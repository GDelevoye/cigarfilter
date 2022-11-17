#include <iostream>  // std::cout
#include <tuple>     // std::get

#include "cigarfilter_plugin.hpp"

CIGARFILTER_PLUGIN(  // A macro to try to make your life easier
    bool filter(CIGAR &cigar, std::string &samline) {
      if (!(cigar.error) && !(cigar.empty)) {
        std::cout << "Cigar string is : " << cigar.as_string << "\n";

        // Let's see if the CIGAR starts or ends with soft-clipped nucleotides
        bool starts_with_clipping = false;
        bool ends_with_clipping = false;

        auto first_group =
            cigar.groups[0];  // This is a std::tuple<char,size_t>
        if (std::get<0>(first_group) == 'S') {
          starts_with_clipping = true;
        }

        auto last_group = cigar.groups[cigar.groups.size() - 1];
        if (std::get<0>(last_group) == 'S') {
          ends_with_clipping = true;
        }

        std::cout << "Does it start with clipping ? " << starts_with_clipping
                  << "\n";
        std::cout << "Does it end with clipping ? " << ends_with_clipping
                  << "\n";

        std::cout << "Now, let's iterate over all groups :\n";

        // A modern for-loop iteration can make it very easy :
        int i = 0;
        for (auto &&[group, nb_elt] :
             cigar.groups) {  // It is for this line, that you have to #include
                              // <tuple>
          std::cout << "Group number " << i << " is of kind " << group << " ";
          std::cout << "and contains " << nb_elt << " elements"
                    << "\n";
          i++;
        }
        std::cout << "Total number of soft-clipped : " << cigar.S << "\n";
      }

      /*
       * Note -->
       * Here, we return something just because the compiler needs a bool.
       * In practice, you will probably use the flag **--shutdown_stdout** with
       * that code anyway
       */

      return false;
    })
