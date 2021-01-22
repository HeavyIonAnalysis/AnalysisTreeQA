if (AnalysisTreeQA_BUNDLED_CUTS)
    message("-- Building bundled AnalysisTreeCuts")

    include(FetchContent)

    FetchContent_Declare(AnalysisTreeCuts
            GIT_REPOSITORY "https://git.cbm.gsi.de/pwg-c2f/analysis/cuts.git"
            #            GIT_VERSION ${AnalysisTreeQA_BUNDLED_CUTS_VERSION}
            GIT_SHALLOW ON
            )
    FetchContent_GetProperties(AnalysisTreeCuts)
    if(NOT analysistreecuts_POPULATED)
        FetchContent_Populate(AnalysisTreeCuts)
        add_subdirectory(${analysistreecuts_SOURCE_DIR} ${analysistreecuts_BINARY_DIR})
    endif()

    add_library(AnalysisTreeCuts_Static INTERFACE)
    target_include_directories(AnalysisTreeCuts_Static INTERFACE ${analysistreecuts_SOURCE_DIR})

endif ()