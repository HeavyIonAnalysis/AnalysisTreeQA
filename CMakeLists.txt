cmake_minimum_required(VERSION 3.14)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(MunkeiVersionFromGitATQA)
version_from_git(
        LOG       ON
        TIMESTAMP "%Y%m%d%H%M%S"
)

project(AnalysisTreeQA VERSION ${VERSION} LANGUAGES CXX)

option(AnalysisTreeQA_BUILD_EXAMPLES "Build AnalysisTreeQA examples (examples/)" ON)
option(AnalysisTreeQA_BUILD_TESTS "Build tests for AnalysisTreeQA" ON)
option(AnalysisTreeQA_BUNDLED_AT "Get and build AnalysisTree" ON)
option(yaml-cpp_BUNDLED "Build bundled yaml-cpp" ON)

set(AnalysisTreeQA_BUNDLED_AT_VERSION "v2.2.6" CACHE STRING "Bundled AnalysisTree version")

# by default build optimized code
if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

# in DEBUG mode make verbose Makefile
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif ()


set(CMAKE_CXX_FLAGS_DEBUG "-O0 -ggdb -g -DDEBUG -D__DEBUG -Wall -Wextra")
set(CMAKE_CXX_FLAGS_Debug "-O0 -ggdb -g -DDEBUG -D__DEBUG -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ftree-vectorize -ffast-math -DNODEBUG")
set(CMAKE_CXX_FLAGS_Release "-O3 -ftree-vectorize -ffast-math -DNODEBUG")
message(STATUS "Using CXX flags for ${CMAKE_BUILD_TYPE}: ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}")

include(ROOT)
include(AnalysisTree)
include(YamlCpp)
include(Boost)

add_subdirectory(src)
add_subdirectory(test)
if(AnalysisTreeQA_BUILD_EXAMPLES)
    add_subdirectory(examples)
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
