# Built as an OBJECT library, not STATIC: every file here registers its task
# type purely as a side effect of a static object's constructor running (see
# ATQA_REGISTER_TASK/ATQA_REGISTER_CUT) - nothing else in the program
# references these symbols directly. A STATIC archive would let the linker
# silently drop such object files; an OBJECT library's objects are always
# passed through when consumed via target_link_libraries.
add_library(AnalysisTreeQABuiltinTasks OBJECT
    EventHeaderQaTask.cpp
    TrackQaTask.cpp
    ParticleQaTask.cpp
    TracksMatchQaTask.cpp
    ParticlesFlowQaTask.cpp
    HistogramQaTask.cpp
    BuiltinCuts.cpp
)
target_link_libraries(AnalysisTreeQABuiltinTasks PUBLIC AnalysisTreeQASteerCore)
