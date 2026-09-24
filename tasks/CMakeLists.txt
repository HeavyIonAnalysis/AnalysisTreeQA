file(GLOB USER_TASK_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
add_library(AnalysisTreeQAUserTasks OBJECT ${USER_TASK_SOURCES})
target_link_libraries(AnalysisTreeQAUserTasks PUBLIC AnalysisTreeQASteerCore)
