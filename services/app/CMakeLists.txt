# ProgramOptions/Application are built as a separate, installable library (not baked directly into the analysistreeqa executable) so that an external repository depending on AnalysisTreeQA as a package (see
# examples/README.md) can reuse the CLI/config-loading logic too, needing only a trivial main.cpp of its own instead of duplicating ProgramOptions/Application.
add_library(AnalysisTreeQAAppCore STATIC
    ProgramOptions.cpp
    Application.cpp
)
target_include_directories(AnalysisTreeQAAppCore PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/AnalysisTreeQA/services/app>
)
target_link_libraries(AnalysisTreeQAAppCore PUBLIC AnalysisTreeQASteerCore Boost::program_options)

install(TARGETS AnalysisTreeQAAppCore EXPORT AnalysisTreeQATargets ARCHIVE DESTINATION lib)
install(FILES Application.hpp ProgramOptions.hpp DESTINATION include/AnalysisTreeQA/services/app)

add_executable(analysistreeqa main.cpp)
target_link_libraries(analysistreeqa
    PRIVATE
        AnalysisTreeQAAppCore
        AnalysisTreeQABuiltinTasks
)

# AnalysisTreeQAExampleTasks (the two worked-example task classes under examples/, see examples/CMakeLists.txt) only exists when AnalysisTreeQA_BUILD_EXAMPLES=ON - we're already inside services/, so
# AnalysisTreeQA_BUILD_SERVICES=ON is guaranteed here (root CMakeLists.txt only add_subdirectory(services) under that condition).
if(AnalysisTreeQA_BUILD_EXAMPLES)
    target_link_libraries(analysistreeqa PRIVATE AnalysisTreeQAExampleTasks)
endif()

install(TARGETS analysistreeqa RUNTIME DESTINATION bin)
