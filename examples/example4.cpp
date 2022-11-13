#include <string> // std::string
#include <iostream> // std::cout
#include <sstream> // std::streamstring
#include <vector>
#include <tuple> // Iterate over the CIGAR's tuples

#include "cigarfilter_plugin.hpp"

CIGARFILTER_PLUGIN(
bool filter(CIGAR &cigar, std::string &samline){
    /*
     * Lambda function that replaces a substring by another, inplace
     * Credit : Michael Mrozek
    */

    auto replace_inplace = [](std::string& str, std::string& from, std::string& to) -> bool {
        size_t start_pos = str.find(from);
        if(start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    };

    // If there is no padding, just print the line as it is
    if(cigar.P == 0){
        return true;
    }

   // Otherwise, let's create a new CIGAR string from scratch :

   // The first step is to create a new list of groups that contains everything but the padding
   std::vector<std::tuple<char,size_t>> newcigar_groups;

   for(auto&& [group,nb_elt] : cigar.groups){
      if(group != 'P'){
          auto newtuple = std::make_tuple(group,nb_elt);
          newcigar_groups.push_back(newtuple);
      }
    }

    // Convert these new groups in a std::string
    std::stringstream newCIGARsstring;
    for(auto&& [group,nb_elt] : newcigar_groups){
         newCIGARsstring << nb_elt << group;
    }
    std::string newCIGARstring = newCIGARsstring.str(); // Convert the streamstring as std::string

    // Change the old CIGAR string by the new one, inplace
    replace_inplace(samline,cigar.as_string,newCIGARstring);

    return true; // samline has been modified inplace, so we can print it
})
