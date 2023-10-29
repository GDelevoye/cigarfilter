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

### 1.1 : Install [mamba](https://github.com/conda-forge/miniforge) (preferred) or [conda](https://docs.conda.io/en/latest/) (deprecated)

[Mamba](https://github.com/conda-forge/miniforge) is a fast/efficient alternative to the well-known [conda](https://docs.conda.io/en/latest/)

```
wget "https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-$(uname)-$(uname -m).sh"
bash Miniforge3-$(uname)-$(uname -m).sh
```

Once it is done, **Restart the terminal**

> If you don't do it, you'll have problems

### 1.2 Build and install the package with mamba

Ensure that you are using mamba instead of conda, and then :

```console
git clone https://github.com/GDelevoye/cigarfilter/
cd cigarfilter
mamba create -n cf cxx-compiler cmake make pip "conan<2" boa -c conda-forge
mamba actvate cf
conda mambabuild ./conda-recipe/ -c conda-forge
mamba install -c local cigarfilter
```

## Step 2 : Check how to use / Test that everything works

Make sure that you are using the "cf" mamba environment that was created in the previous script

The main program :

```console
(base) delevoye@Dell-G15-5530:~/cigarfilter$ mamba activate cf
(cf) delevoye@Dell-G15-5530:~/cigarfilter$ cigarfilter -h
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

The companion software to handle the plugins : 


```console
(cf) delevoye@Dell-G15-5530:~/cigarfilter$ cigarfilter_config -h
  * help :
	 Displays this help

  * pluginpath :
	 Displays the path were filters are stored. Manually modify them only at your own risks.

  * list :
	 Lists all available filters.

  * add :
	 Adds a filter from a .cpp file (see README online at https://github.com/GDelevoye/cigarfilter).

  * remove :
	 Removes a filter. Example : "cigarfilter_config remove default".

  * clean :
	 Cleans all logs and temporary files.

  * purge :
	 Cleans all logs, temporary files and all filters except the default one.
```

Run the automated tests to make sure (among other things) that the two software can communicate correctly : 

```console
(cf) delevoye@Dell-G15-5530:~/cigarfilter$ cigarfilter_test
Running main() from /home/delevoye/.conan/data/gtest/1.12.1/_/_/build/bd0690086619c54f81a8365940843110bf637ce7/src/googletest/src/gtest_main.cc
[==========] Running 20 tests from 3 test suites.
[----------] Global test environment set-up.
[----------] 12 tests from cigar
[ RUN      ] cigar.t_listdigit
[       OK ] cigar.t_listdigit (0 ms)
[ RUN      ] cigar.t_eq
[       OK ] cigar.t_eq (0 ms)
[ RUN      ] cigar.t_empty
[       OK ] cigar.t_empty (0 ms)
[ RUN      ] cigar.t_S
[       OK ] cigar.t_S (0 ms)
[ RUN      ] cigar.t_D
[       OK ] cigar.t_D (0 ms)
[ RUN      ] cigar.t_I
[       OK ] cigar.t_I (0 ms)
[ RUN      ] cigar.t_X
[       OK ] cigar.t_X (0 ms)
[ RUN      ] cigar.t_N
[       OK ] cigar.t_N (0 ms)
[ RUN      ] cigar.t_P
[       OK ] cigar.t_P (0 ms)
[ RUN      ] cigar.t_H
[       OK ] cigar.t_H (0 ms)
[ RUN      ] cigar.t_M
[       OK ] cigar.t_M (0 ms)
[ RUN      ] cigar.complex_1
[       OK ] cigar.complex_1 (0 ms)
[----------] 12 tests from cigar (0 ms total)

[----------] 2 tests from plugin_helpers
[ RUN      ] plugin_helpers.plugindir
[       OK ] plugin_helpers.plugindir (16 ms)
[ RUN      ] plugin_helpers.get_default_plugin
[       OK ] plugin_helpers.get_default_plugin (17 ms)
[----------] 2 tests from plugin_helpers (33 ms total)

[----------] 6 tests from ExampleTest
[ RUN      ] ExampleTest.iterate_groups_t
[       OK ] ExampleTest.iterate_groups_t (51 ms)
[ RUN      ] ExampleTest.default_t
[       OK ] ExampleTest.default_t (51 ms)
[ RUN      ] ExampleTest.example1_t
[       OK ] ExampleTest.example1_t (50 ms)
[ RUN      ] ExampleTest.example2_t
[       OK ] ExampleTest.example2_t (50 ms)
[ RUN      ] ExampleTest.example3_t
[       OK ] ExampleTest.example3_t (51 ms)
[ RUN      ] ExampleTest.example4_t
ERROR : Could not remove the tmp file in fixture "StdoutFixture"
[       OK ] ExampleTest.example4_t (16 ms)
[----------] 6 tests from ExampleTest (272 ms total)

[----------] Global test environment tear-down
[==========] 20 tests from 3 test suites ran. (306 ms total)
[  PASSED  ] 20 tests.
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
