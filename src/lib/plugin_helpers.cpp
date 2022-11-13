//
// Created by guillaume on 09/10/22.
//
#include "plugin_helpers.hpp"

#include <boost/dll/import.hpp> // for import_alias
#include <boost/filesystem.hpp>

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

namespace dll = boost::dll;

namespace cf{

    /* This returns the stdout of a subprocess. Problem: stderr is ignored
     * Can probably be improved
     * Copy-pasted from :
     * https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
     */
    std::string exec(const char* cmd) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }

    // relies on a correct installation of cigarfilter_config
    std::string get_plugin_dir() {
        auto plugindir = exec("cigarfilter_config pluginpath");
        return plugindir;
    }

    /*
     * returns a boost plugin object, that allows dependency injection from a dynamically linked library
     * See https://www.boost.org/doc/libs/master/doc/html/boost_dll/tutorial.html
     */
    boost::shared_ptr<cigarfilter_plugin> get_plugin(std::string name){
        auto plugin_dir = boost::filesystem::path(get_plugin_dir());
        boost::shared_ptr<cigarfilter_plugin> plugin;

        // Boost is supposed to be able to decorate the name of the library (see dll::load_mode::append_decorations)
        // In practice however, it just does not work for me (bug ?), so here is a workaround
        auto suffixes = {".so",".lib",".dll",".a", ".dylib", ""};
        std::string right_extension = "";
        for(auto extension : suffixes){
            auto suffixed_path = plugin_dir / std::string(name + std::string(extension));
            if(boost::filesystem::exists(suffixed_path)){
                right_extension = std::string(extension);
                break;
            }
        }

        // Better to beg forgivenesss than ask permission : let's try what we have (nb: the last try is "")

        plugin = dll::import_symbol<cigarfilter_plugin>(
                plugin_dir / (name + right_extension),
                "plugin"                          // name of the symbol to import
        ); // If this fails, boost throws an exception saying that the file is missing

        /*
        * Important note about "import" vs "import_symbol" in the dll module :
        * A renaming has occured recently in boost to avoid collision with the std C++20 "import" func
        * See : https://github.com/robotology/blocktest/issues/47
        * This here compiles fine with boost 1.65. It may fail with older versions
        */

        return plugin;
    }

}