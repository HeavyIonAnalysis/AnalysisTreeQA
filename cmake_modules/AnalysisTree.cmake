find_package(AnalysisTree QUIET)
set(AnalysisTree_BUILD_EXAMPLES OFF)

include(FetchContent)
FetchContent_Declare(AnalysisTree
        GIT_REPOSITORY  "https://github.com/HeavyIonAnalysis/AnalysisTree.git"
        GIT_TAG         "v1.0.2"
        UPDATE_DISCONNECTED ${UPDATE_DISCONNECTED}
        )

FetchContent_GetProperties(AnalysisTree)
if(NOT analysistree_POPULATED)
    FetchContent_Populate(AnalysisTree)
    add_subdirectory(${analysistree_SOURCE_DIR} ${analysistree_BINARY_DIR})
endif()

get_target_property(AnalysisTreeBase_INCLUDE_DIR AnalysisTreeBase INCLUDE_DIRECTORIES)
list(APPEND PROJECT_INCLUDE_DIRECTORIES ${AnalysisTreeBase_INCLUDE_DIR})
get_target_property(AnalysisTreeInfra_INCLUDE_DIR AnalysisTreeInfra INCLUDE_DIRECTORIES)
list(APPEND PROJECT_INCLUDE_DIRECTORIES ${AnalysisTreeInfra_INCLUDE_DIR})

#set(AnalysisTree_INSTALL_DIR ${EXTERNAL_INSTALL_DIR})
#set(AnalysisTree_INCLUDE_DIR ${AnalysisTree_INSTALL_DIR}/include)
#set(AnalysisTree_LIBRARY_DIR ${AnalysisTree_INSTALL_DIR}/lib)
#
#ExternalProject_Add(AnalysisTree_Ext
#        GIT_REPOSITORY  "https://github.com/viktorklochkov/AnalysisTree.git"
#        GIT_TAG         "v1.0.2"
#        UPDATE_DISCONNECTED ${UPDATE_DISCONNECTED}
#        SOURCE_DIR      "${EXTERNAL_DIR}/AnalysisTree_src"
#        BINARY_DIR      "${EXTERNAL_DIR}/AnalysisTree_build"
#        INSTALL_DIR     "${AnalysisTree_INSTALL_DIR}"
#        CMAKE_ARGS
#        "-DCMAKE_INSTALL_PREFIX=${AnalysisTree_INSTALL_DIR}"
#        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
#        "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}"
#        )
#
#find_library(AnalysisTree_LIB NAMES libAnalysisTreeBase.so libAnalysisTreeInfra.so HINTS ${AnalysisTree_LIBRARY_DIR} REQUIRED)
#add_library(AnalysisTree SHARED IMPORTED)
#set_target_properties(AnalysisTree PROPERTIES IMPORTED_LOCATION ${AnalysisTree_LIB})
#
#
#list(APPEND PROJECT_DEPENDENCIES AnalysisTree_Ext)
#list(APPEND PROJECT_LINK_DIRECTORIES ${AnalysisTree_LIBRARY_DIR})
#list(APPEND PROJECT_INCLUDE_DIRECTORIES ${AnalysisTree_INCLUDE_DIR})
