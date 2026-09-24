add_executable(analysistreeqa main.cpp ProgramOptions.cpp Application.cpp)
target_link_libraries(analysistreeqa
    PRIVATE
        AnalysisTreeQASteerCore
        AnalysisTreeQABuiltinTasks
        Boost::program_options
)

# AnalysisTreeQAUserTasks (user-private tasks under the top-level tasks/,
# see tasks/CMakeLists.txt) only exists when AnalysisTreeQA_BUILD_TASKS=ON,
# which itself requires AnalysisTreeQA_BUILD_SERVICES=ON (enforced in the
# root CMakeLists.txt).
if(AnalysisTreeQA_BUILD_TASKS)
    target_link_libraries(analysistreeqa PRIVATE AnalysisTreeQAUserTasks)
endif()

install(TARGETS analysistreeqa RUNTIME DESTINATION bin)
