#include <iostream> // std::cout
#include "cigarfilter_plugin.hpp"

CIGARFILTER_PLUGIN( // A macro to try to make your life easier
bool filter(CIGAR &cigar, std::string &samline){
    if(!(cigar.error) && !(cigar.empty)){
        auto nb_matches = cigar.M + cigar.eq;
        auto sequence_length = nb_matches + cigar.X + cigar.I;
        auto reference_length = nb_matches + cigar.X + cigar.D + cigar.S + cigar.H + cigar.N;

        auto percent_of_identity = (2.0 * nb_matches) / ((1.0*(sequence_length + reference_length)));

        std::cout << percent_of_identity << "\n";
    }
    return false;
})
