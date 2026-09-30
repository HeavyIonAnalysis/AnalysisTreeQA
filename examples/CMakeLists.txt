include(AnalysisTree)

if(Boost_FOUND)
    include_directories(${PROJECT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR} ${AnalysisTree_BINARY_DIR}/include)
    link_directories(${PROJECT_LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS})
else()
    include_directories(${PROJECT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} ${AnalysisTree_BINARY_DIR}/include)
    link_directories(${PROJECT_LINK_DIRECTORIES})
endif()

if(PROJECT_LINK_DIRECTORIES)
    include_directories(${PROJECT_INCLUDE_DIRECTORIES} ${AnalysisTree_BINARY_DIR}/include)
    link_directories(${PROJECT_LINK_DIRECTORIES})
endif()

set(EXECUTABLES example
                )

foreach(EXE ${EXECUTABLES})
    add_executable(${EXE} ${EXE}.cpp)
    add_dependencies(${EXE} AnalysisTreeQA)
    target_link_libraries(${EXE} ${ROOT_LIBRARIES} AnalysisTreeQA AnalysisTreeBase AnalysisTreeInfra)
    install (TARGETS ${EXE} RUNTIME DESTINATION bin)
    install (FILES ${EXE}.cpp DESTINATION share)
endforeach()

# Two worked-example services/ task classes (ExampleEventHeaderTask, ExampleParticleTask, see examples/README.md), each booking one H1/H2/TProfile directly in Init(). Only buildable together with services/ itself
# (needs AnalysisTreeQASteerCore/ATQA_REGISTER_TASK), unlike the classic example.cpp macro above which never needs it, hence the separate guard here rather than gating the whole examples/ directory on it.
if(AnalysisTreeQA_BUILD_SERVICES)
    add_library(AnalysisTreeQAExampleTasks OBJECT
            ExampleEventHeaderTask.cpp
            ExampleParticleTask.cpp)
    target_link_libraries(AnalysisTreeQAExampleTasks PUBLIC AnalysisTreeQASteerCore)
endif()
