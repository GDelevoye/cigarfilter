# Must be ran in this directory
cd ..
mkdir -p build && cd build
cmake .. && make all
cd ..
rm -vf code_coverage.info
rm -rvf code_coverage_report
./build/bin/cigarfilter_t
lcov --directory ./build/ --capture --output-file ./code_coverage.info -rc lcov_branch_coverage=1
genhtml code_coverage.info --branch-coverage --output-directory ./code_coverage_report/
dir=$(pwd)
firefox $dir/code_coverage_report$dir/src/lib/index.html
