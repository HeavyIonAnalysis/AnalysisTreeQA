# Minimum 3.5 version is determined by cmake -E copy_if_different command
# that was used to copy headers to the build directory:
# multiple files are supported since cmake 3.5
cmake_minimum_required(VERSION 3.5)
project(AnalysisTreeQA CXX)
set(PROJECT_VERSION 1.1)

# by default build optimized code
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RELEASE)
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

# in DEBUG mode make verbose Makefile
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif ()

if(CMAKE_CXX_STANDARD LESS 17)
    find_package(Boost REQUIRED)
endif()

if(Boost_FOUND)
    message(STATUS "Boost version ${Boost_VERSION_STRING} is found!")
    message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")
endif()

set(AnalysisTreeQA_BUILD_TESTS OFF CACHE BOOL "Build tests for the AnalysisTree")
set(AnalysisTreeQA_BUILD_EXAMPLES ON CACHE BOOL "Build examples for the AnalysisTree")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -g -DDEBUG -D__DEBUG -Wall -Wextra --coverage")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ftree-vectorize -ffast-math -DNODEBUG")
message(STATUS "Using CXX flags for ${CMAKE_BUILD_TYPE}: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH ${ROOTSYS})

find_package(AnalysisTree QUIET)
find_package(ROOT REQUIRED RIO)

message(STATUS "Using ROOT: ${ROOT_VERSION} <${ROOT_CONFIG}>")
include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIR} ${ROOT_INCLUDE_DIRS})
include(${ROOT_USE_FILE})

set(EXTERNAL_DIR ${CMAKE_BINARY_DIR}/external)
set(EXTERNAL_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/external)

if(NOT AnalysisTree_FOUND)
    include(cmake_modules/AnalysisTree.cmake)
endif()

include_directories(${CMAKE_SOURCE_DIR} ${AnalysisTree_INCLUDE_DIR} ${PROJECT_INCLUDE_DIRECTORIES})

add_subdirectory(src)

if(AnalysisTreeQA_BUILD_EXAMPLES)
    add_subdirectory(tasks)
endif()

if(AnalysisTreeQA_BUILD_TESTS)
    include(GoogleTest)
    enable_testing()
    add_subdirectory(test)
endif()

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/AnalysisTreeQA/AnalysisTreeQAConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

export(EXPORT AnalysisTreeQATargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/AnalysisTreeQA/AnalysisTreeQATargets.cmake")

message(STATUS "AnalysisTreeQATargets: ${AnalysisTreeQATargets}")
        
set(ConfigPackageLocation lib/cmake/AnalysisTreeQA)

set(AnalysisTreeQA_INCLUDE_DIR "include")
set(AnalysisTreeQA_LIBRARY_DIR "lib")

configure_package_config_file(
        AnalysisTreeQAConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/AnalysisTreeQA/AnalysisTreeQAConfig.cmake
        INSTALL_DESTINATION ${ConfigPackageLocation}
        PATH_VARS AnalysisTreeQA_INCLUDE_DIR AnalysisTreeQA_LIBRARY_DIR
)

install(EXPORT AnalysisTreeQATargets
        FILE
        AnalysisTreeQATargets.cmake
        DESTINATION
        ${ConfigPackageLocation}
        )

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/AnalysisTreeQA/AnalysisTreeQAConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/AnalysisTreeQA/AnalysisTreeQAConfigVersion.cmake"
        DESTINATION
        ${ConfigPackageLocation}
        COMPONENT
        Devel
        )

