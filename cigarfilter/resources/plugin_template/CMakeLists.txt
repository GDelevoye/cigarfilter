project(plugin)

cmake_minimum_required(VERSION 3.5.2)
set(CMAKE_BUILD_TYPE Release)

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
        boost/1.75.0
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


file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

add_library(${PROJECT_NAME} SHARED
        ${SOURCES}
        src/cigarfilter_plugin.hpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_11)
conan_target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES PREFIX "")
