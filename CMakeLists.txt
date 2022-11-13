project(cigarfilter)
cmake_minimum_required(VERSION 3.5.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS --coverage)

#set(-static-libgcc -static-libstdc++)

#############################################################################
# Run "conan install" from CMake :
# See : https://github.com/conan-io/cmake-conan Run conan install

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})


# It requires to have either this file already copied on the repository, or to download it
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
    message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
    file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
            "${CMAKE_BINARY_DIR}/conan.cmake"
            TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES
        gtest/1.12.1
        boost/1.80.0
        lyra/1.5.1
        spdlog/1.9.2
        GENERATORS cmake)

conan_cmake_autodetect(settings)

add_definitions("-std=c++17")

conan_cmake_install(PATH_OR_REFERENCE .
        BUILD missing
        REMOTE conancenter
        SETTINGS ${settings})

#############################################################################

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

#############################################################################

# Ship the header in the plugin template
file(COPY ${CMAKE_SOURCE_DIR}/include/cigarfilter_plugin.hpp DESTINATION ${CMAKE_SOURCE_DIR}/cigarfilter/resources/plugin_template/src/)

# cigarfilter executable

include_directories(include)
include_directories(src/lib)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/lib/*.cpp")

add_executable(${PROJECT_NAME}
        src/lib/cigarfilter.hpp
        src/executable/main.cpp
        ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
conan_target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})

# Examples

# DEFAULT FILTER
add_library(default SHARED
        examples/default.cpp)
target_compile_features(default PRIVATE cxx_std_11)
conan_target_link_libraries(default ${CONAN_LIBS})
SET_TARGET_PROPERTIES(default PROPERTIES PREFIX "")


# ITERATE GROUPS (TUTO N°1)
add_library(iterate_groups SHARED
        examples/iterate_groups.cpp)
target_compile_features(iterate_groups PRIVATE cxx_std_11)
conan_target_link_libraries(iterate_groups ${CONAN_LIBS})
SET_TARGET_PROPERTIES(iterate_groups PROPERTIES PREFIX "")

# EXAMPLES

add_library(example1 SHARED
        examples/example1.cpp)
target_compile_features(example1 PRIVATE cxx_std_11)
conan_target_link_libraries(example1 ${CONAN_LIBS})
SET_TARGET_PROPERTIES(example1 PROPERTIES PREFIX "")

add_library(example2 SHARED
        examples/example2.cpp)
target_compile_features(example2 PRIVATE cxx_std_11)
conan_target_link_libraries(example2 ${CONAN_LIBS})
SET_TARGET_PROPERTIES(example2 PROPERTIES PREFIX "")

add_library(example3 SHARED
        examples/example3.cpp)
target_compile_features(example3 PRIVATE cxx_std_11)
conan_target_link_libraries(example3 ${CONAN_LIBS})
SET_TARGET_PROPERTIES(example3 PROPERTIES PREFIX "")

add_library(example4 SHARED
        examples/example4.cpp)
target_compile_features(example4 PRIVATE cxx_std_11)
conan_target_link_libraries(example4 ${CONAN_LIBS})
SET_TARGET_PROPERTIES(example4 PROPERTIES PREFIX "")


## Tests
enable_testing()

add_executable(${PROJECT_NAME}_test
        ${SOURCES}
        src/tests/CIGAR.t.cpp
        src/tests/plugin_helpers.t.cpp
        src/tests/examples.t.cpp
        src/lib/cigarfilter.hpp
        include/cigarfilter_plugin.hpp
        )

target_compile_features(${PROJECT_NAME}_test PRIVATE cxx_std_11)
conan_target_link_libraries(${PROJECT_NAME}_test cigarfilter ${CONAN_LIBS})

# Installation...

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(TARGETS ${PROJECT_NAME}_test DESTINATION bin)

# Install example plugins
install(TARGETS default DESTINATION ${CMAKE_SOURCE_DIR}/cigarfilter/resources/plugins) 
install(TARGETS iterate_groups DESTINATION ${CMAKE_SOURCE_DIR}/cigarfilter/resources/plugins)
install(TARGETS example1 DESTINATION ${CMAKE_SOURCE_DIR}/cigarfilter/resources/plugins) 
install(TARGETS example2 DESTINATION ${CMAKE_SOURCE_DIR}/cigarfilter/resources/plugins)
install(TARGETS example3 DESTINATION ${CMAKE_SOURCE_DIR}/cigarfilter/resources/plugins) 
install(TARGETS example4 DESTINATION ${CMAKE_SOURCE_DIR}/cigarfilter/resources/plugins) 

