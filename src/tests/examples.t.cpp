//
// Created by guillaume on 08/11/22.
//

// Checking the examples

#include "gtest/gtest.h"
#include <iostream>
#include <bits/stdc++.h>
#include <sstream>  // std::stringstream
#include <vector>
#include <fstream>
#include "CIGAR.hpp"
#include "plugin_helpers.hpp"
#include "boost/filesystem.hpp"
#include <cstdio>

class ExampleTest : public ::testing::Test{ // For tests where stdout is redirected to a tmp file;
    protected:
        ExampleTest()
        {

        }

        ~ExampleTest()
        {
            auto plugin_dir = cf::get_plugin_dir();
            auto tmp_path = boost::filesystem::path(plugin_dir).parent_path() / "test_data" / "tmp_teststdout";
            auto tmp_path_str = tmp_path.string();
            auto result = remove(tmp_path_str.c_str());
            if(result != 0){
                std::cerr << "ERROR : Could not remove the tmp file in fixture \"StdoutFixture\"" << std::endl;
            }
        }
};


// helper function for the other examples
void run_stdout_example(bool print_header,
                        std::string filename_input,
                        std::string filename_expected_output,
                        std::string testname){

    auto plugin_dir = cf::get_plugin_dir();
    auto input_path = boost::filesystem::path(plugin_dir).parent_path() / "test_data" / filename_input;
    auto path_to_expected_output = boost::filesystem::path(plugin_dir).parent_path() / "test_data" / filename_expected_output;
    auto tmp_path = boost::filesystem::path(plugin_dir).parent_path() / "test_data" / "tmp_teststdout";

    // Read the expected output :

    std::ifstream expected_output_ifstream(path_to_expected_output.string());
    std::stringstream buffer_expected;
    buffer_expected << expected_output_ifstream.rdbuf();
    std::string expected_output = buffer_expected.str();

    // ...

    std::stringstream cmd;

    // Except in Windows, I suppose that more or less all platforms are UNIX-like...
    #if (defined (_WIN32) || defined (_WIN64))
        cmd << "type";
    #else
        cmd << "cat";
    #endif

    //The cmd will be : cat/type [path to input] | cigarfilter -f [testname] --print_header [print_header] > [path to tmp]

    cmd << " " << input_path.string() << " | cigarfilter -f "<< testname << " --print_header " << print_header << " > " << tmp_path.string();
    auto as_str = cmd.str();

    // Execute the cmd...
    cf::exec(as_str.c_str());

    // Read the actual output :
    std::ifstream actual_output_ifstream(tmp_path.string());
    std::stringstream buffer_actual;
    buffer_actual << actual_output_ifstream.rdbuf();
    std::string actual_output = buffer_actual.str();

    ASSERT_EQ(actual_output,expected_output);
}

TEST_F(ExampleTest,iterate_groups_t){
    run_stdout_example(false,"test_input_iterate_groups.sam","expected_iterate_groups.sam","iterate_groups");
}

TEST_F(ExampleTest,default_t){
    run_stdout_example(true,"test_input_base.sam","expected_default_filter.sam","default");
}

TEST_F(ExampleTest,example1_t){ // Output only reads that have at least one intron whose size is >= 15 nt
    run_stdout_example(true,"test_input_example1.sam","expected_example1.sam","example1");
}

TEST_F(ExampleTest,example2_t){ // Keep only reads whose last and/or first group is >= 10 clipped nucleotides (hard or soft)
    run_stdout_example(true,"test_input_example2.sam","expected_example2.sam","example2");
}

// This one deserves a special treatment because we're talking about floats, and the output may be platform-dependent
TEST_F(ExampleTest,example3_t){

    auto plugin_dir = cf::get_plugin_dir();
    auto input_path = boost::filesystem::path(plugin_dir).parent_path() / "test_data" / "test_input_base.sam";
    auto tmp_path = boost::filesystem::path(plugin_dir).parent_path() / "test_data" / "tmp_teststdout";

    std::vector<float> expected_output = {
            1.0,
            0.714829,
            0.700491,
            1.0,
            1.0,
            1.0,
            0.387574,
            1.0,
            0.862745,
            0.806154,
            0.842857,
            0.826962,
            0.944724,
            0.934993,
            0.746929,
            0.421053,
            0.421053,
            0.99734,
            0.998384,
            0.997512,
            1.0,
            0.741543,
            0.993808,
            0.756129,
            0.137546,
            0.997519,
            0.558646,
            1.0,
            0.891417,
            0.856643,
            0.902954,
            0.995334,
            0.584098,
            0.408284,
            0.868661,
            0.43913,
            0.596226,
            0.287582,
            0.897494,
            1.0,
            0.47323,
            0.833333,
            0.959302,
            0.995363,
            0.995693,
            0.623188,
            0.851648,
            1.0,
            0.896882,
            0.947368
    };

    std::stringstream cmd;

    // Except in Windows, I suppose that more or less all platforms are UNIX-like...
    #if (defined (_WIN32) || defined (_WIN64))
        cmd << "type";
    #else
        cmd << "cat";
    #endif

    //The cmd will be : cat/type [path to input] | cigarfilter -f print_stdout --print_header false > [path to tmp]

    cmd << " " << input_path.string() << " | cigarfilter -f example3 --print_header false > " << tmp_path.string();
    auto as_str = cmd.str();
    cf::exec(as_str.c_str());

    // Now let's read line by line the file produced, and check if it matches

    int i = 0;
    std::ifstream infile(tmp_path.string());
    for (std::string line; std::getline(infile, line);){
        float actual_value = std::stof(line);
        float expected_value = expected_output[i];
        i++;

        ASSERT_NEAR(actual_value,expected_value,0.01);
    }

    ASSERT_EQ(i,expected_output.size());
}

TEST_F(ExampleTest,example4_t){ // Remove all padding
    run_stdout_example(true,"test_input_example4.sam","expected_example4.sam","example4");
}



