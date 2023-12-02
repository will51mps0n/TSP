//3E33912F8BAA7542FC4A1585D2DB6FE0312725B9
#include <iostream>
#include <unistd.h>
#include <getopt.h>
#include <string>
#include <cstdlib>
#include <iomanip>
#include "MSTZoo.h"
#include "FASTTSPZoo.h"
enum class Mode
{
    MST,
    FASTTSP,
    OPTTSP
};

struct CommandLineOptions {
    Mode mode;
    bool help = false;
};

CommandLineOptions parseArgs(int argc, char* argv[]) {
    CommandLineOptions options;
    int option_index = 0;
    bool modeSet = false;

    static struct option long_options[] = {
        {"mode", required_argument, 0, 'm'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    int c;
    while ((c = getopt_long(argc, argv, "m:h", long_options, &option_index)) != -1) {
        switch (c) {
            case 'm':
                modeSet = true;
                if (std::string(optarg) == "MST") options.mode = Mode::MST;
                else if (std::string(optarg) == "FASTTSP") options.mode = Mode::FASTTSP;
                else if (std::string(optarg) == "OPTTSP") options.mode = Mode::OPTTSP;
                else {
                    std::cerr << "Invalid mode specified." << std::endl;
                    exit(1);
                }
                break;
            case 'h':
                options.help = true;
                std::cout << "Usage: ./zoo --mode {MST|FASTTSP|OPTTSP}\n"
                          << "Options:\n"
                          << "  -m, --mode    Mode of operation\n"
                          << "  -h, --help    Display this help and exit\n";
                exit(0);
                break;
            case '?':
            default:
                std::cerr << "Usage: ./zoo --mode {MST|FASTTSP|OPTTSP}" << std::endl;
                exit(1);
        }
    }

    if (!modeSet) {
        std::cerr << "Mode not set. Use -m or --mode." << std::endl;
        exit(1);
    }

    return options;
}


int main(int argc, char* argv[]) 
{
std::ios_base::sync_with_stdio(false);
CommandLineOptions options = parseArgs(argc, argv);
std::cout << std::setprecision(2) << std::fixed;
uint32_t numCages = 0;
std::vector<Vertex> allVertices = readInput(numCages);

if (options.mode == Mode::MST) {
    MSTZoo zoo(numCages, allVertices);
    zoo.PrimMST();
    return 0;
}
else if (options.mode == Mode::FASTTSP) 
{
FASTZoo zoo(numCages, allVertices);
std::vector<size_t> parentCages = zoo.createOptimalPath();
zoo.outputPath(parentCages);
}
return 0;
}