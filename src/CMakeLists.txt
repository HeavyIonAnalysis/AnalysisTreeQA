set(SOURCES
    Manager.cpp
    EntryConfig.cpp
    Axis.cpp
    Task.cpp
)

string(REPLACE ".cpp" ".hpp" HEADERS "${SOURCES}")
list(APPEND HEADERS "VariantMagic.hpp" "Utils.hpp")

add_library(AnalysisTreeQA STATIC ${SOURCES} G__AnalysisTreeQA.cxx)

if(Boost_FOUND)
  include_directories(${CMAKE_SOURCE_DIR}/src ${PROJECT_INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIR})
  link_directories(${PROJECT_LINK_DIRECTORIES} ${Boost_LIBRARY_DIRS})
else()
  include_directories(${CMAKE_SOURCE_DIR}/src ${PROJECT_INCLUDE_DIRECTORIES})
  link_directories(${PROJECT_LINK_DIRECTORIES})
endif()

add_dependencies(AnalysisTreeQA ${PROJECT_DEPENDENCIES})

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
