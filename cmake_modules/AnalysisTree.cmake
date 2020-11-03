find_package(AnalysisTree QUIET)
set(AnalysisTree_BUILD_EXAMPLES OFF)

include(FetchContent)
FetchContent_Declare(AnalysisTree
        GIT_REPOSITORY  "https://github.com/viktorklochkov/AnalysisTree.git"
        GIT_TAG         "dev"
        UPDATE_DISCONNECTED ${UPDATE_DISCONNECTED}
        )

FetchContent_GetProperties(AnalysisTree)
if(NOT analysistree_POPULATED)
    FetchContent_Populate(AnalysisTree)
    add_subdirectory(${analysistree_SOURCE_DIR} ${analysistree_BINARY_DIR})
endif()

list(APPEND PROJECT_DEPENDENCIES AnalysisTreeInfra)
get_target_property(AnalysisTreeBase_INCLUDE_DIR AnalysisTreeBase INCLUDE_DIRECTORIES)
list(APPEND PROJECT_INCLUDE_DIRECTORIES ${AnalysisTreeBase_INCLUDE_DIR})
get_target_property(AnalysisTreeInfra_INCLUDE_DIR AnalysisTreeInfra INCLUDE_DIRECTORIES)
