if (AnalysisTreeQA_BUNDLED_AT)
    message("-- Building bundled AnalysisTree")
    include(FetchContent)

    FetchContent_Declare(
            AnalysisTree
            GIT_REPOSITORY "https://github.com/viktorklochkov/AnalysisTree.git"
#            GIT_TAG ${AnalysisTreeQA_BUNDLED_AT_VERSION}
            GIT_TAG cmake
            GIT_SHALLOW ON
    )
    FetchContent_MakeAvailable(AnalysisTree)
    list(APPEND PROJECT_INCLUDE_DIRECTORIES ${AnalysisTree_BINARY_DIR}/include)
    message(STATUS "AT: ${AnalysisTree_BINARY_DIR} ${AnalysisTree_SOURCE_DIR}")
else()
    find_package(AnalysisTree REQUIRED)
    list(APPEND CMAKE_PREFIX_PATH ${ANALYSISTREE_HOME})
    list(APPEND CMAKE_PREFIX_PATH $ENV{ANALYSISTREE_HOME})
    list(APPEND PROJECT_INCLUDE_DIRECTORIES ${AnalysisTree_INCLUDE_DIR})
endif()

list(APPEND PROJECT_LINK_LIBRARIES AnalysisTreeBase AnalysisTreeInfra)
