/**
 * \file CIGAR.hpp
 * \brief Implementation of the CIGAR parsing
 * \warning Contains some hard-coded compile-time optimizations and relies on a correct CIGAR
 * \author DELEVOYE Guillaume
 * \version 0.1
 * \date Nov 2022
 */

#ifndef BAMFILTER_CIGAR_HPP
#define BAMFILTER_CIGAR_HPP

#include <vector>
#include <map>
#include <set>

#include "cigarfilter_plugin.hpp"

namespace cf {

/**
 * @brief Compilation-time optimization so that we don't compute pows of floats (or double) at runtime
 * @see "listdigits_to_number()"
 * @note Although it is hardcoded, it is technically impossible for a group to be >= 1000000000 nt long
 */
static size_t pow10[10 /**< Which exponent */] = {
        1, 10, 100, 1000, 10000,
        100000, 1000000, 10000000, 100000000, 1000000000,
};

/**
 * @brief From a vector of int [1,4,5,6] (for instance), returns 1456
 * @param listdigits Vector of integers (not templated for other types)
 * @return size_t
 * @details Optimized based on the fact that the vector size is inferior to 10
 */
size_t listdigits_to_number(std::vector<int> listdigits);


/**
 * @brief Returns the total number of insertions, deletions, etc... In a .sam line
 * @param inputstring : A .sam line
 * @details If a group name is not in {"M","=","I","D","X","H","S","P","N"}, cigar.error = true
 * @warning The implementation relies mostly on the assumption that the CIGAR string is correct
 * @todo Better handling of errors
 */
    cigarfilter_plugin::CIGAR get_CIGAR(const std::string &inputstring);
}

#endif //BAMFILTER_CIGAR_HPP
