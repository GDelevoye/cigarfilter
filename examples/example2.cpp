#include <tuple>  // For std::get

#include "cigarfilter_plugin.hpp"

CIGARFILTER_PLUGIN(bool filter(CIGAR &cigar, std::string &samline) {
  if (!(cigar.error) && !(cigar.empty)) {
    auto first_group = cigar.groups[0];  // This is a std::tuple<char,size_t>
    auto group_type =
        std::get<0>(first_group);  // The first element of the tuple can be
                                   // either '=', 'M', 'X' etc.
    auto group_size = std::get<1>(
        first_group);  // This is the number of elements in the group

    bool starts_with_more10_clipping =
        ((group_type == 'S') || (group_type == 'H')) && (group_size >= 10);

    auto last_group = cigar.groups[cigar.groups.size() - 1];  // The last group
    group_type = std::get<0>(last_group);
    group_size = std::get<1>(last_group);

    bool ends_with_more10_clipping =
        ((group_type == 'S') || (group_type == 'H')) && (group_size >= 10);

    return (starts_with_more10_clipping || ends_with_more10_clipping);
  }

  // You may want to add a std::cerr here, to warn the user if the CIGAR is
  // incorrect or empty Mind to #include<iostream> if you plan to do so

  return false;
})
