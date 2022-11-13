//
// Created by guillaume on 09/10/22.
//

#ifndef BAMFILTER_CLI_HELPERS_HPP
#define BAMFILTER_CLI_HELPERS_HPP

#include <string>
#include <boost/shared_ptr.hpp>
#include <vector>

#include "cigarfilter_plugin.hpp"

namespace cf {
    /**
     * @brief Returns a shared pointer to an object that contains the user-defined filter
     * @param name Name of the plugin (no .so .dll etc)
     * @return boost::shared_ptr<cigarfilter_plugin>
     */
    boost::shared_ptr<cigarfilter_plugin> get_plugin(std::string name);
    /**
     * @brief Returns the directory where the plugins are. Requires a correct installation of cigarfilter_config
     */
    std::string get_plugin_dir();
    /**
     * @brief Executes a command and returns its standard output. See <a href=/https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po>/ This SO post</a>
     * @warning Does not check for the standard error
     */
    std::string exec(const char* cmd);
}
#endif
