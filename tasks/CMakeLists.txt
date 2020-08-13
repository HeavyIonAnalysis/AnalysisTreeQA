include_directories(${PROJECT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(cbm_qa cbm_qa.cpp)
add_dependencies(cbm_qa AnalysisTreeQA)
target_link_libraries(cbm_qa AnalysisTreeQA ${ROOT_LIBRARIES})

install (TARGETS cbm_qa RUNTIME DESTINATION bin)