# Built as an OBJECT library, not STATIC: every file here registers its task
# type purely as a side effect of a static object's constructor running (see
# ATQA_REGISTER_TASK/ATQA_REGISTER_CUT) - nothing else in the program
# references these symbols directly. A STATIC archive would let the linker
# silently drop such object files; an OBJECT library's objects are always
# passed through when consumed via target_link_libraries.
add_library(AnalysisTreeQABuiltinTasks OBJECT
    TrackQaTask.cpp
    HistogramQaTask.cpp
    BuiltinCuts.cpp
)
target_include_directories(AnalysisTreeQABuiltinTasks PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include/AnalysisTreeQA/services/steer/tasks>
)
target_link_libraries(AnalysisTreeQABuiltinTasks PUBLIC AnalysisTreeQASteerCore)

# Exported so an external repository can link the built-in task types (see
# services/README.md, "Using AnalysisTreeQA from your own repository").
install(TARGETS AnalysisTreeQABuiltinTasks EXPORT AnalysisTreeQATargets)
install(FILES TrackQaTask.hpp HistogramQaTask.hpp
        DESTINATION include/AnalysisTreeQA/services/steer/tasks)
