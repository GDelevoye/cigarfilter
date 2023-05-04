[![CircleCI](https://dl.circleci.com/status-badge/img/gh/GDelevoye/cigarfilter/tree/main.svg?style=svg)](https://dl.circleci.com/status-badge/redirect/gh/GDelevoye/cigarfilter/tree/main)

# cigarfilter

***

<p align="center">  
  <b> A fast C++ tool to filter .bam files by CIGAR strings </b><br>
  <a href="https://github.com/GDelevoye/cigarfilter/wiki"> Wiki</a> |
  <a href="https://github.com/GDelevoye/cigarfilter/wiki/2.-Quick-start">Quick start tutorial</a> |
  <a href="https://github.com/GDelevoye/cigarfilter/wiki/4.-Create-your-own-filters">Examples</a>
  <br><br>
</p>

***

> This page contains only explanations for a quick start. For more advanced usage, please see [the wiki](https://github.com/GDelevoye/cigarfilter/wiki)


Please do not hesitate to [open an issue](https://github.com/GDelevoye/cigarfilter/issues) if you find any bug, want a feature, or if you have any question. 

PRs are welcome. If you want to contribute, some guidelines and proposals are avaiable [here](https://github.com/GDelevoye/cigarfilter/wiki/7.-Contribute).

# Motivation 

* The **.sam** file format is a widely-spread standard in bioinformatics to store aligned NGS reads.
  *Its full description can be found [here](https://samtools.github.io/hts-specs/SAMv1.pdf)

* The 5th field of the file (0-indexing) stores what is called a **"CIGAR string"** (see [1](https://sites.google.com/site/bioinformaticsremarks/bioinfo/sam-bam-format/what-is-a-cigar), [2](https://samtools.github.io/hts-specs/SAMv1.pdf), [3](https://jef.works/blog/2017/03/28/CIGAR-strings-for-dummies/) for details)

  * It corresponds to how the read mapped on the reference genome.

![BAM_presentation](https://user-images.githubusercontent.com/29506993/196156645-bdf7428a-5b9d-45c7-b299-c32f70a773bb.jpg)

People often handle the CIGAR strings with tricky **regex/awk/grep[...] combinations**. But there are cases where it gets virtually impossible. Example : 

> ** _"I want to filter all reads that have only one intron whose size is between 25nt and 50nt, while the rest of the read matches perfectly, except maybe some soft-clipping at the edges."_ **

In such situations, people use **Python/R/Perl scipts**, that are much more expressive. But it can be very slow for big datasets (>100Gb). 

**C++ would have a faster runtime**, but it comes with many hassles - especially when you want to deploy it in a cloud/cluster computing environment. 

This projects aims to simplify the development/deployment of such filters coded in C++.

# Quick start

## Step 1. Installation

### Requirement:

* [A C++17 compliant compiler](https://en.cppreference.com/w/cpp/compiler_support)
* CMake >= 3.25
* Python, pip

### One-liner installation (from sources)

```console
pip install conan==1.59.0 && git clone https://github.com/GDelevoye/cigarfilter/ && cd cigarfilter && mkdir -p build && cd build && cmake .. && make install && cd .. && pip install .
```

```
## Step 2. Check that your installation worked

```console
user@computer:~$ cigarfilter -h
USAGE:
  cigarfilter [-?|-h|--help] [-f|--filter <filter name>] [-p|--print_header <bool>]

Display usage information.

OPTIONS, ARGUMENTS:
  -?, -h, --help          
  -f, --filter <filter name>
                          Use "cigarfilter_config help" for more info.
  -p, --print_header <bool>
                          When set to false, the header is not printed. <default: TRUE>

```

## Step 3. Copy-paste the template in a .cpp source file

```cpp
#include "cigarfilter_API.hpp" // Don't worry if this file is not in your directory

CIGARFILTER_PLUGIN ( // A macro that wraps your code in a cigarfilter plugin. 
bool filter(CIGAR &cigar, std::string &samline){ 
   /*
    * Your implementation goes here
    */
    // If you return true, the read passes the filter (i.e it will be printed)
}) // Don't forget to close the parenthesis
```

## Step 4. Implement your filter

Here is a simple example of implementation : 

```cpp
#include "cigarfilter_API.hpp"

CIGARFILTER_PLUGIN ( 
bool filter(CIGAR &cigar, std::string &samline){ 
   if(cigar.D >= 5){ // If the read contains 5 deletions or more, don't print it...
          return false; 
   }
   else{
      return true;
   }
})
```

See [here](https://github.com/GDelevoye/cigarfilter/wiki/4.-Create-your-own-filters) for the documentation of the CIGAR class and more examples

## Step 5. Compile and install your filter

```bash
cigarfilter_config add [your_implementation.cpp] [your_filter_name] 
# This is where the dark magic happens
```

## Step 6. Use your filter


cigarfilter was designed to work well with samtools :

```bash
samtools view -h ./input.bam | cigarfilter -f your_filter_name | \
		samtools view -bS > filtered_output.bam
```

You can also use it directly on .sam files :


```bash
cat ./input.sam | cigarfilter -f your_filter_name > filtered_output.sam
```

By default, cigarfilter transmits the header. If you want to avoid that behaviour you can use "--print_header false" : 

```bash
# The header will not be present in filtered_output.sam
cat ./input.sam | cigarfilter -f your_filter_name --print_header false > filtered_output.sam
```


