add_library(AnalysisTreeQASteerCore STATIC
    Config.cpp
    Run.cpp
    TaskFactory.cpp
    CutFactory.cpp
)
target_include_directories(AnalysisTreeQASteerCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIRECTORIES}>
    $<BUILD_INTERFACE:${AnalysisTree_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include/AnalysisTreeQA/services/steer>
)
target_link_libraries(AnalysisTreeQASteerCore PUBLIC AnalysisTreeQA AnalysisTreeBase AnalysisTreeInfra ${AnalysisTreeQA_YAMLCPP_TARGET})

# Exported so an external repository can depend on AnalysisTreeQA as a
# package and still get the TaskFactory/CutFactory infrastructure - see
# services/README.md, "Using AnalysisTreeQA from your own repository".
install(TARGETS AnalysisTreeQASteerCore EXPORT AnalysisTreeQATargets ARCHIVE DESTINATION lib)
install(FILES Config.hpp Run.hpp TaskFactory.hpp CutFactory.hpp
        DESTINATION include/AnalysisTreeQA/services/steer)

add_subdirectory(tasks)
