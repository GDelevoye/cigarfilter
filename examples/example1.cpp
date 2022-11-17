#include <tuple>  // To iterate over the groups of the CIGAR, that are tuples

#include "cigarfilter_plugin.hpp"

CIGARFILTER_PLUGIN(bool filter(CIGAR &cigar, std::string &samline) {
  if (!(cigar.error) && !(cigar.empty)) {
    for (const auto &[group, nb_elt] : cigar.groups) {
      if ((group == 'N') && (nb_elt) >= 15) {
        return true;
      }
    }
  }
  return false;
})
