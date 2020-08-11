
if(Boost_FOUND)
  include_directories(${PROJECT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR} ${Boost_INCLUDE_DIR})
  link_directories(${PROJECT_LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS})
else()
  include_directories(${PROJECT_INCLUDE_DIRECTORIES} ${CMAKE_CURRENT_SOURCE_DIR})
  link_directories(${PROJECT_LINK_DIRECTORIES})
endif()


add_executable(cbm_qa cbm_qa.cpp)
add_dependencies(cbm_qa AnalysisTreeQA ${PROJECT_DEPENDENCIES})
target_link_libraries(cbm_qa ${ROOT_LIBRARIES} AnalysisTreeQA AnalysisTreeBase AnalysisTreeInfra)

install (TARGETS cbm_qa RUNTIME DESTINATION bin)