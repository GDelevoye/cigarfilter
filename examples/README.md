# Overview

* [A friendly CIGAR structure that you can use in your filters](#cigar)
* [Examples](#examples)
   * [Iterate the CIGAR group by group](#iteration)
   * [Let pass only perfectly matched reads](#perfectmatch)
   * [Keep only reads that have at least one intron whose size is >= 15 nt](#intron)
   * [Let pass only reads that do start or end with >=10 clipped nucleotides (hard or soft)](#clipping)
   * [Output a homemade percentage of identity to make statistics](#statistics)
   * [Remove all padding](#padding)

# <a name="cigar">A friendly CIGAR structure that you can use in your filters</a>

You can work directly on the raw .sam line in your filters. **But you will probably save time using the already-coded CIGAR struct** defined in "cigarfilter_plugin.hpp" :

```cpp
struct CIGAR {
   size_t M = 0; // Match or mismatch
   size_t eq = 0; // Match ("=")
   size_t I = 0; // Insertion
   size_t D = 0; // Deletion
   size_t X = 0; // Substitution
   size_t H = 0; // Hard-clipped
   size_t S = 0; // Soft-clipped
   size_t P = 0; // Padded
   size_t N = 0; // Skipped (or "Spliced")
 
   bool empty = false; // The CIGAR is empty. Set to true when the 5th field is "*"
   bool error = false; // true if something wrong happened. ⚠️ : Only a small subset of errors are caught.

   std::string as_string = "";

   // Stored as groups
   std::vector<std::tuple<char,size_t>> groups;
};
```

 Everything is public in this structure, which allows you to modify/hack things how you want.
 
 The CIGAR groups are stored for you in a STL vector : 
 
 ```cpp
 std::vector<std::tuple<char,size_t>> groups;
 ```

 You can therefore iterate group by group, random access it, and more generally do [everything you can do with a vector in C++11] [https://en.cppreference.com/w/cpp/container/vector).

# <a name="examples"> Examples </a>

When programming, the better way to explain is often by showing a few lines of code. Here are some examples of what you can do with the CIGAR struct

> How to reproduce the examples ?
>
> 1. Copy-paste the text in any file you want (say, **implementation.cpp**), anywhere you want
> 2. Use :  
> 
> ```console
> cigarfilter_config add implementation.cpp YOUR_FILTER_NAME
> ```

## <a name="iteration"> Iterate over a CIGAR, group by group </a>

Remember that the CIGAR groups are stored in a STL vector of tuples : 

```cpp
std::vector<std::tuple<char,size_t>> groups;
```

This allows you to iterate over the groups : 

```cpp
// A modern for-loop iteration can make it very easy :
for(auto&& [group,nb_elt] : cigar.groups){ // mind to #include <tuple> if you use this
    std::cout << "Group " << group<< " ";
    std::cout << " contains " << nb_elt << " elements" << "\n";
}
```

You can also access the last, first or *nth* element, as tuples : 

```cpp
auto last_group = cigar.groups[cigar.groups.size()-1]; // Last element
auto type_group = std::get<0>(last_group); // Mind to #include <tuple> for this to work
auto size_group = std::get<1>(last_group);
```

Example :

```cpp
#include <iostream> // std::cout
#include <tuple> // std::get
#include "cigarfilter_plugin.hpp"

CIGARFILTER_PLUGIN( // A macro to try to make your life easier
bool filter(CIGAR &cigar,std::string &samline){
    if( !(cigar.error) && !(cigar.empty)){
        std::cout << "Cigar string is : " << cigar.as_string << "\n";

        // Let's see if the CIGAR starts or ends with soft-clipped nucleotides
        bool starts_with_clipping = false;
        bool ends_with_clipping = false;

        auto first_group = cigar.groups[0]; // This is a std::tuple<char,size_t>
        if(std::get<0>(first_group) == 'S'){
           starts_with_clipping = true;
        }

        auto last_group = cigar.groups[cigar.groups.size()-1];
        if(std::get<0>(last_group) == 'S'){
           ends_with_clipping = true;
        }

        std::cout << "Does it start with clipping ? " << starts_with_clipping << "\n";
        std::cout << "Does it end with clipping ? " << ends_with_clipping << "\n";

        std::cout << "Now, let's iterate over all groups :\n";

        // A modern for-loop iteration can make it very easy :
        int i = 0;
        for(auto&& [group,nb_elt] : cigar.groups){ // It is for this line, that you have to #include <tuple>
            std::cout << "Group number " << i << " is of kind " << group<< " ";
            std::cout << "and contains " << nb_elt << " elements" << "\n";
            i++;
        }
        std::cout << "Total number of soft-clipped : " << cigar.S << "\n";
    }

    /*
     * Note -->
     * Here, we return something just because the compiler needs a bool.
    */

    return false;
})
```

Here is an example of **what it does in practice if a file contains only one read whose CIGAR string is 10S160M15S :**

```console
user@computer:$ cigarfilter_config add myfilter.cpp myfilter
user@computer:$ samtools view -h input.bam | cigarfilter -f myfilter --print_header false
Cigar string is 10S160M15S
Does it start with clipping ? true
Does it ends with clipping ? true
Now, let's iterate over all groups :
Group number 0 is of kind S and contains 10 elements
Group number 1 is of kind M and contains 10 elements
Group number 2 is of kind S and contains 15 elements
Total number of soft-clipped : 25
user@computer:$
```


## <a name="perfectmatch"> Let pass only perfectly matched reads </a>

A perfect match is a case where all base in the sequence match with the reference, and reciprocally. 

One way of doing it is simply by ensuring that only "=" or "M" are present in the CIGAR. Another way, less straightforward, is to to compute a Percentage of IDentity (PID), and let pass only reads with a PID = 1.

```cpp

// NB : This is actually the default filter 

#include <tuple> // For std::get
#include "cigarfilter_plugin.hpp"

CIGARFILTER_PLUGIN(
bool filter(CIGAR &cigar,std::string &samline){
    if(!(cigar.error) && !(cigar.empty)){
        auto first_group = cigar.groups[0]; // This is a std::tuple<char,size_t>
        auto group_type = std::get<0>(first_group); // The first element of the tuple can be either '=', 'M', 'X' etc.
        auto group_size = std::get<1>(first_group); // This is the number of elements in the group

        bool starts_with_more10_clipping = ( (group_type == 'S') || (group_type == 'H') ) && (group_size >= 10);

        auto last_group = cigar.groups[cigar.groups.size()-1]; // The last group
        group_type = std::get<0>(last_group);
        group_size = std::get<1>(last_group);

        bool ends_with_more10_clipping = ( (group_type == 'S') || (group_type == 'H') ) && (group_size >= 10);

        return (starts_with_more10_clipping || ends_with_more10_clipping);
    }

    // You may want to add a std::cerr here, to warn the user if the CIGAR is incorrect or empty
    // Mind to #include<iostream> if you plan to do so

    return false;
}) // end of wrapping macro
```

Since this is the default filter, you can already use it

```console
user@computer:$ cat your_reads.sam | cigarfilter > output.sam
```

If you work with samtools and .bam files : 

```console
user@computer:$ samtools view your_reads.bam | cigarfilter | samtools view -bS > output.bam
```

## <a name="intron"> Keep only reads that have at least one intron whose size is >= 15 nt </a>

This is typically a task that can be done very easily in C++ but would not be readable using a regex or grep/awk script, and very slow using python/R scripts :

```cpp
#include "cigarfilter_plugin.hpp"
#include <tuple> // To iterate over the groups of the CIGAR, that are tuples

CIGARFILTER_PLUGIN(
bool filter(CIGAR &cigar, std::string &samline){
    if(!(cigar.error) && !(cigar.empty)){
        for(const auto&[group,nb_elt] : cigar.groups){
            if( (group == 'N') && (nb_elt) >= 15){
                return true;
            }
        }
    }
    return false;
})

```

## <a name="clipping"> Let pass only reads that do start or end with >=10 clipped nucleotides (hard or soft) </a>

```cpp
#include <tuple> // For std::get

bool filter(const CIGAR &cigar, const std::string &samline){
    if(!(cigar.error) && !(cigar.empty)){

        auto first_group = cigar.groups[0]; // This is a std::tuple<char,size_t>
        auto group_type = std::get<0>(first_group); // The first element of the tuple can be either '=', 'M', 'X' etc.
        auto group_size = std::get<1>(first_group); // This is the number of elements in the group

        bool starts_with_more10_clipping = ( (group_type == 'S') || (group_type == 'H') ) && (group_size >= 10);

        auto last_group = cigar.groups[cigar.groups.size()]; // The last group 
        group_type = std::get<0>(last_group); 
        group_size = std::get<1>(last_group);       

        bool ends_with_more10_clipping = ( (group_type == 'S') || (group_type == 'H') ) && (group_size >= 10);

        return (starts_with_more10_clipping && ends_with_more10_clipping);
    }

    // You may want to add a std::cerr here, to warn the user if the CIGAR is incorrect or empty
    // Mind to #include<iostream> if you plan to do so

    return false; 
}
```
## <a name="statistics"> Output a homemade percentage of identity to make statistics </a>

```cpp
#include <iostream> // std::cout

bool filter(const CIGAR &cigar, const std::string &samline){
    auto nb_matches = cigar.M + cigar.eq;

    auto sequence_length = nb_matches + cigar.X + cigar.I;
    auto reference_length = nb_matches + cigar.X + cigar.D + cigar.S + cigar.H + cigar.N;

    auto percent_of_identity = (2.0 * nb_matches) / (1.0*(sequence_length + reference_length));
    
    std::cout << percent_of_identity << "\n";

    return false; // It does not matter, you just have to output a bool here
}
```

You can then print all percentages of identity without the header lines, by using the **--print_header** flag : 

```console
user@computer:$ cigarfilter_config add myfilter myfilter.cpp
user@computer:$ samtools view -h input.bam | cigarfilter -f myfilter --print_header false
1
1
1
0.99876
1
1
0.32564
[...]
```

##  <a name="padding">Remove all padding</a>

Here, we will re-write the CIGAR string. 


```cpp
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

```
