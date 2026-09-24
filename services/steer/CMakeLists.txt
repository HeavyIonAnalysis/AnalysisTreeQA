add_library(AnalysisTreeQASteerCore STATIC
    Config.cpp
    Run.cpp
    TaskFactory.cpp
    CutFactory.cpp
)
target_include_directories(AnalysisTreeQASteerCore PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
target_link_libraries(AnalysisTreeQASteerCore PUBLIC AnalysisTreeQA AnalysisTreeBase AnalysisTreeInfra ${AnalysisTreeQA_YAMLCPP_TARGET})

add_subdirectory(tasks)
