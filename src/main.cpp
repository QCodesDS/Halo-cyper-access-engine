#include <chrono>
#include <iostream>
#include <sstream>

#include "anomaly/AnomalyEngine.h"
#include "cli/Printer.h"
#include "indexing/IndexManager.h"
#include "query/QueryEngine.h"
#include "storage/CsvLoader.h"
#include "storage/DataStore.h"
#include "storage/Deduplicator.h"
#include "utils/StringUtils.h"

int main() {
    std::cout << "=== Halo Cyber Access Engine ===" << std::endl;
    std::cout << "Type 'help' for available commands" << std::endl;
    std::cout << std::endl;

    DataStore store;
    initDataStore(store);

    IndexManager indexMgr;
    indexMgr.isBuilt = false;

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!getline(std::cin, line)) {
            break;
        }

        // Trim line
        line = trim(line);

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Split command and arguments
        int argc = 0;
        std::string* argv = split(line, ' ', argc);

        if (argc == 0) {
            if (argv != nullptr) {
                delete[] argv;
            }
            continue;
        }

        std::string command = argv[0];

        // Load command
        if (strEquals(command, "load") && argc >= 2) {
            std::string filepath = argv[1];
            LoadResult loadResult = load(filepath, store);

            int numDuplicates = deduplicate(store);

            std::stringstream ss;
            ss << "Loaded " << loadResult.loaded << " records (" << loadResult.skipped << " skipped, " << numDuplicates
               << " duplicates removed)";
            printInfo(ss.str());

            // Build indexes
            if (store.count > 0) {
                buildAllIndexes(indexMgr, store);
            }
        }
        // Query commands
        else if (strEquals(command, "query") || strEquals(command, "top")) {
            // Reconstruct query string from remaining tokens
            std::string queryStr = line;

            if (!indexMgr.isBuilt) {
                printError("No data loaded. Use 'load <filepath>' first.");
            } else {
                executeQuery(queryStr, indexMgr.hashIdx, indexMgr.sortedIdx);
            }
        }
        // Detect command
        else if (strEquals(command, "detect")) {
            if (!indexMgr.isBuilt) {
                printError("No data loaded. Use 'load <filepath>' first.");
            } else {
                std::string typeFilter = "all";
                bool valid = true;

                if (argc >= 2) {
                    std::string arg1 = argv[1];
                    if (strEquals(arg1, "anomaly")) {
                        if (argc >= 3) {
                            std::string arg2 = argv[2];
                            if (arg2.rfind("--type=", 0) == 0) {
                                std::string typeVal = arg2.substr(7);
                                if (strEquals(typeVal, "all") || strEquals(typeVal, "threshold") ||
                                    strEquals(typeVal, "behavior") || strEquals(typeVal, "session") ||
                                    strEquals(typeVal, "advanced")) {
                                    typeFilter = typeVal;
                                } else {
                                    printError("Invalid type filter. Supported types: all, threshold, behavior, session, advanced");
                                    valid = false;
                                }
                            } else {
                                printError("Unknown argument: " + arg2 + ". Usage: detect anomaly [--type=all|threshold|behavior|session|advanced]");
                                valid = false;
                            }
                        }
                    } else {
                        printError("Unknown argument: " + arg1 + ". Did you mean 'detect anomaly'?");
                        valid = false;
                    }
                }

                if (valid) {
                    auto startDetect = std::chrono::high_resolution_clock::now();
                    AnomalyList results;
                    initAnomalyList(results);
                    runAnomalyDetection(results, store, indexMgr.hashIdx, typeFilter);
                    auto endDetect = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> diff = endDetect - startDetect;

                    printAnomalyReport(results);

                    std::stringstream ss;
                    ss << "Detection completed in " << diff.count() << "s";
                    printInfo(ss.str());

                    clearAnomalyList(results);
                }
            }
        }
        // Help command
        else if (strEquals(command, "help")) {
            std::cout << "Available commands:" << std::endl;
            std::cout << "  load <filepath>                          - Load CSV file" << std::endl;
            std::cout << "  query user <uid> <t_start> <t_end>       - User journey query" << std::endl;
            std::cout << "  query resource <rid> <t_start> <t_end>   - Resource journey query" << std::endl;
            std::cout << "  top resources <t_start> <t_end>          - Top resources by count" << std::endl;
            std::cout << "  detect anomaly [--type=...]              - Run anomaly detection" << std::endl;
            std::cout << "  help                                     - Show this help message" << std::endl;
            std::cout << "  exit                                     - Exit program" << std::endl;
        }
        // Exit command
        else if (strEquals(command, "exit")) {
            printInfo("Memory freed. Goodbye.");
            if (argv != nullptr) {
                delete[] argv;
            }
            // Clear indexes
            if (indexMgr.isBuilt) {
                clearAllIndexes(indexMgr);
            }
            clear(store);
            break;
        }
        // Unknown command
        else {
            printError("Unknown command: " + command);
        }

        if (argv != nullptr) {
            delete[] argv;
        }
    }

    // Make sure store is cleaned up
    if (store.count > 0) {
        clear(store);
    }

    return 0;
}
