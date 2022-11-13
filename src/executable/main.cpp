#include <string>
#include <istream>
#include <iostream>

#include <lyra/lyra.hpp> // argument parser
#include <bits/stdc++.h>

#include "cigarfilter.hpp"
#include "cigarfilter_plugin.hpp"

int main(int argc, char* argv[]) {
    bool showHelp = false;
    bool print_header = true;
    std::string filtername = "default";

    auto cli = lyra::cli();
    cli.add_argument(lyra::help(showHelp));

    cli.add_argument(lyra::opt(filtername, "filter name")
                             .name("-f").name("--filter")
                             .optional()
                             .help("Use \"cigarfilter_config help\" for more info.")
                             );

    cli.add_argument(lyra::opt(print_header, "bool")
                             .name("-p").name("--print_header")
                             .optional()
                             .help("When set to false, the header is not printed. <default: TRUE>")
                             .choices(true,false));

    // Parse :
    auto result = cli.parse({argc, argv});

    // Check that the arguments were valid :
    if (!result) {
        std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        std::cerr << "Use cigarfilter -h to display help." << std::endl;
        return EXIT_FAILURE;
    }

    // If being asked, display help then quit
    if (showHelp){
        std::cout << cli << "\n";
        return 0;
    }

    // Get the user-defined plugin (dependency injection)
    auto plugin = cf::get_plugin(filtername);

    // https://www.geeksforgeeks.org/fast-io-for-competitive-programming/
    // This is probably the most important optimization trick here
    std::ios_base::sync_with_stdio(false);

    /*
     * This disables the synchronization between the C and C++ standard streams.
     * By default, all standard streams are synchronized
     * If you disable the synchronization, then C++ streams are allowed to have their own independent buffers
     */

    std::cin.tie(nullptr);

    /*
     * This unties cin from cout.
     * Tied streams ensure that one stream is flushed automatically before each I/O operation on the other stream.
     * By default cin is tied to cout to ensure a sensible user interaction.
     * But, here, we don't want that because there is no human user
     */

    for (std::string line; std::getline(std::cin, line);) {
        if (line[0] == '@') { // It's a header
            if(print_header){
                std::cout << line + '\n';
            }
        }
        else{
            auto cigar = cf::get_CIGAR(line);
            auto passes = plugin->filter(cigar, line);
            if(passes){
                std::cout << line+'\n'; // std::endl; would flush the stream repeatedly.
            }

        }
    }

    return EXIT_SUCCESS;
}