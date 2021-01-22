if (AnalysisTreeQA_BUNDLED_AT)
    message("-- Building bundled AnalysisTree")
    include(FetchContent)

    FetchContent_Declare(
            AnalysisTree
            GIT_REPOSITORY "https://github.com/HeavyIonAnalysis/AnalysisTree.git"
            GIT_TAG ${AnalysisTreeQA_BUNDLED_AT_VERSION}
            GIT_SHALLOW ON
    )
    FetchContent_MakeAvailable(AnalysisTree)

#    FetchContent_GetProperties(AnalysisTree)
#    if(NOT analysistree_POPULATED)
#        FetchContent_Populate(AnalysisTree)
#        add_subdirectory(${analysistree_SOURCE_DIR} ${analysistree_BINARY_DIR})
#    endif()
    message(STATUS "DFGDFGDFGD ${analysistree_SOURCE_DIR}")

else()
    list(APPEND CMAKE_PREFIX_PATH ${ANALYSISTREE_HOME})
    list(APPEND CMAKE_PREFIX_PATH $ENV{ANALYSISTREE_HOME})
    find_package(AnalysisTree REQUIRED)
    list(APPEND PROJECT_INCLUDE_DIRECTORIES ${AnalysisTree_INCLUDE_DIR})
endif()
