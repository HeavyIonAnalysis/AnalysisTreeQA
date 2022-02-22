set(SOURCES
    EntryConfig.cpp
    Task.cpp
    YamlReader.cpp
    Axis.cpp)

string(REPLACE ".cpp" ".hpp" HEADERS "${SOURCES}")
list(APPEND HEADERS "BasicQA.hpp")

include_directories(${PROJECT_INCLUDE_DIRECTORIES})

add_library(AnalysisTreeQA SHARED ${SOURCES} G__AnalysisTreeQA.cxx)
target_link_libraries(AnalysisTreeQA PUBLIC ${PROJECT_LINK_LIBRARIES} yaml-cpp)
add_dependencies(AnalysisTreeQA AnalysisTreeInfra)
target_include_directories(AnalysisTreeQA PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

ROOT_GENERATE_DICTIONARY(G__AnalysisTreeQA
        ${HEADERS}
        LINKDEF AnalysisTreeQALinkDef.h
        OPTIONS -I${CMAKE_BINARY_DIR}/include
        )


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
