#include <string>
#include <vector>

#include "CIGAR.hpp"
#include <iostream>

namespace cf{

size_t listdigits_to_number(std::vector<int> listdigits){
    size_t final_digit = 0;
    size_t lsize = listdigits.size();

    for (size_t i = 0; i < lsize; i++){
        final_digit += listdigits[i] * pow10[lsize-i-1];
    }
    return final_digit;
}

cigarfilter_plugin::CIGAR get_CIGAR(const std::string &inputstring){
    int tab_count = 0;
    size_t i = 0;
    cigarfilter_plugin::CIGAR cigar{};

    std::vector<int> listdigits;

    while(tab_count != 5){ // Go until we've encountered the 5th tab character ...
        if(inputstring[i] == '\t'){
            tab_count ++;
        }
        i++;
    }

    // ... After the 5th tab, that's the CIGAR ...

    // ... It can be empty ...
    char c = inputstring[i];

    if(c=='*'){ // ... in which case we can return ...
        cigar.empty = true;
        cigar.as_string = "*";
        return cigar;
    }

    // ... Most often however, it will rather contain one or more groups :

    while(c != '\t'){ // While a part of the CIGAR remains to be read
        cigar.as_string.push_back(c);
        if(isdigit(c)){             // If the character is a number :
            int this_integer = (int) c - 48; // ASCII to digit
            listdigits.push_back(this_integer);
        }

        else{             // End of the CIGAR group
            int number = listdigits_to_number(listdigits);
            // Using a hashtable rather than a switch is preferable for maintenance but not for performances
            switch(c){
                case 'M':
                    cigar.M += number;
                    cigar.groups.push_back(std::make_tuple<char,size_t>('M',number));
                    break;
                case '=':
                    cigar.eq += number;
                    cigar.groups.push_back(std::make_tuple<char,size_t>('=',number));
                    break;
                case 'I':
                    cigar.I += number;
                    cigar.groups.push_back(std::make_tuple<char,size_t>('I',number));
                    break;
                case 'D' :
                    cigar.D += number;
                    cigar.groups.push_back(std::make_tuple<char,size_t>('D',number));
                    break;
                case 'X':
                    cigar.X += number;
                    cigar.groups.push_back(std::make_tuple<char,size_t>('X',number));
                    break;
                case 'H':
                    cigar.H += number;
                    cigar.groups.push_back(std::make_tuple<char,size_t>('H',number));
                    break;
                case 'S':
                    cigar.S += number;
                    cigar.groups.push_back(std::make_tuple<char,size_t>('S',number));
                    break;
                case 'P':
                    cigar.P += number;
                    cigar.groups.push_back(std::make_tuple<char,size_t>('P',number));
                    break;
                case 'N':
                    cigar.N += number;
                    cigar.groups.push_back(std::make_tuple<char,size_t>('N',number));
                    break;
                default:
                    // WARNING : This does not catch all errors at all !!
                    cigar.error = true;
                    break;
            }
            listdigits.clear();
        }
        i++;
        c = inputstring[i];
        // end of while loop
    } // We have reached the end of the CIGAR
    return cigar;
} // end of the function get_cigar()
} // end of the namespace