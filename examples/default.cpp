#include "cigarfilter_plugin.hpp"

CIGARFILTER_PLUGIN(bool filter(CIGAR &cigar, std::string &samline) {
  if (!(cigar.error) && !(cigar.empty)) {
    auto nb_matches = cigar.M + cigar.eq;
    auto sequence_length = nb_matches + cigar.X + cigar.I;
    auto reference_length =
        nb_matches + cigar.X + cigar.D + cigar.S + cigar.H + cigar.N;

    auto percent_of_identity =
        (2.0 * nb_matches) / (1.0 * (sequence_length + reference_length));

    /*
     * NB : As in many other programming languages (including python...),
     * you should never directly check if a float equals another in C++
     * See
     * https://www.geeksforgeeks.org/problem-in-comparing-floating-point-numbers-and-how-to-compare-them-correctly/
     */

    if (percent_of_identity >= (1 - 1e-9)) {  // This is the closest we have to
                                              // "if(percent_identity == 1.0)"
      return true;
    } else {
      return false;
    }
  }
  return false;
})  // end of wrapping macro
