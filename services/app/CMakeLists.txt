# ProgramOptions/Application are built as a separate, installable library
# (not baked directly into the analysistreeqa executable) so that an
# external repository depending on AnalysisTreeQA as a package (see
# services/README.md, "Using AnalysisTreeQA from your own repository") can
# reuse the CLI/config-loading logic too, needing only a trivial main.cpp of
# its own instead of duplicating ProgramOptions/Application.
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

# AnalysisTreeQAUserTasks (user-private tasks under the top-level tasks/,
# see tasks/CMakeLists.txt) only exists when AnalysisTreeQA_BUILD_TASKS=ON,
# which itself requires AnalysisTreeQA_BUILD_SERVICES=ON (enforced in the
# root CMakeLists.txt).
if(AnalysisTreeQA_BUILD_TASKS)
    target_link_libraries(analysistreeqa PRIVATE AnalysisTreeQAUserTasks)
endif()

install(TARGETS analysistreeqa RUNTIME DESTINATION bin)
