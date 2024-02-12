if (AnalysisTreeQA_BUNDLED_AT)
    message("-- Building bundled AnalysisTree")
    include(FetchContent)

    FetchContent_Declare(
            AnalysisTree
            GIT_REPOSITORY "https://github.com/lubynets/AnalysisTree.git"
            GIT_TAG "dev"
            GIT_SHALLOW "OFF"
    )
    FetchContent_MakeAvailable(AnalysisTree)
else()
    list(APPEND CMAKE_PREFIX_PATH ${ANALYSISTREE_HOME})
    list(APPEND CMAKE_PREFIX_PATH $ENV{ANALYSISTREE_HOME})
    find_package(AnalysisTree REQUIRED)
    list(APPEND PROJECT_INCLUDE_DIRECTORIES ${AnalysisTree_INCLUDE_DIR})
endif()
