cmake_minimum_required(VERSION 3.0)
project(AnalysisTreeQA CXX)
set(PROJECT_VERSION 1.0)

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "Using C++${CMAKE_CXX_STANDARD}")
include(ExternalProject)

if(CMAKE_CXX_STANDARD LESS 17)
    find_package(Boost REQUIRED)
endif()

if(Boost_FOUND)
    message(STATUS "Boost version ${Boost_VERSION_STRING} is found!")
    message(STATUS "Boost include dir: ${Boost_INCLUDE_DIRS}")
    add_definitions(-DUSEBOOST)
endif()

# by default build optimized code with debug symbols
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE RELWITHDEBINFO)
endif ()

# in DEBUG mode make verbose Makefile
if (CMAKE_BUILD_TYPE MATCHES DEBUG)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif ()

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -g -DDEBUG -D__DEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ftree-vectorize -ffast-math -DNODEBUG")
message(STATUS "Using CXX flags for ${CMAKE_BUILD_TYPE}: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
list(APPEND CMAKE_PREFIX_PATH ${ROOTSYS})
find_package(AnalysisTree)
find_package(ROOT REQUIRED RIO)

if (ROOT_FOUND)
    message(STATUS "Using ROOT: ${ROOT_VERSION} <${ROOT_CONFIG}>")
    include_directories(${CMAKE_SOURCE_DIR} ${ROOT_INCLUDE_DIR} ${ROOT_INCLUDE_DIRS})
    include(${ROOT_USE_FILE})
endif(ROOT_FOUND)

set(EXTERNAL_DIR ${CMAKE_BINARY_DIR}/external)
set(EXTERNAL_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/external)

if(NOT AnalysisTree_FOUND)
    include(cmake_modules/analysistree.cmake)
endif()

include(cmake_modules/cuts.cmake)

include_directories(${CMAKE_SOURCE_DIR} ${AnalysisTree_INCLUDE_DIR} ${PROJECT_INCLUDE_DIRECTORIES})

add_subdirectory(src)
#add_subdirectory(test)
add_subdirectory(tasks)

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

