set(SOURCES
    EntryConfig.cpp
    Task.cpp
)

string(REPLACE ".cpp" ".hpp" HEADERS "${SOURCES}")
list(APPEND HEADERS "BasicQA.hpp")

add_library(AnalysisTreeQA SHARED ${SOURCES} G__AnalysisTreeQA.cxx)

include_directories(${CMAKE_SOURCE_DIR}/src ${PROJECT_INCLUDE_DIRECTORIES})
link_directories(${PROJECT_LINK_DIRECTORIES})

if(PROJECT_DEPENDENCIES)
  add_dependencies(AnalysisTreeQA ${PROJECT_DEPENDENCIES})
endif()

ROOT_GENERATE_DICTIONARY(G__AnalysisTreeQA ${HEADERS} LINKDEF AnalysisTreeQALinkDef.h)
target_link_libraries(AnalysisTreeQA ${ROOT_LIBRARIES})

install(TARGETS AnalysisTreeQA EXPORT AnalysisTreeQATargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

install(
        FILES
        ${HEADERS}
        DESTINATION
        include/AnalysisTreeQA
        COMPONENT
        Devel
)

set(PCM_FILE_NAME libAnalysisTreeQA)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}_rdict.pcm"
        DESTINATION
        lib
        OPTIONAL
)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}.rootmap"
        DESTINATION
        lib
        OPTIONAL
)
