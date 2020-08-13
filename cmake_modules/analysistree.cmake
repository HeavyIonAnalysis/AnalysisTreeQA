set(AnalysisTree_INSTALL_DIR ${EXTERNAL_INSTALL_DIR})
set(AnalysisTree_INCLUDE_DIR ${AnalysisTree_INSTALL_DIR}/include)
set(AnalysisTree_LIBRARY_DIR ${AnalysisTree_INSTALL_DIR}/lib)

ExternalProject_Add(AnalysisTree_Ext
        GIT_REPOSITORY  "https://github.com/viktorklochkov/AnalysisTree.git"
        GIT_TAG         "master"
        UPDATE_DISCONNECTED ${UPDATE_DISCONNECTED}
        SOURCE_DIR      "${EXTERNAL_DIR}/AnalysisTree_src"
        BINARY_DIR      "${EXTERNAL_DIR}/AnalysisTree_build"
        INSTALL_DIR     "${AnalysisTree_INSTALL_DIR}"
        CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${AnalysisTree_INSTALL_DIR}"
        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
        "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}"
        )

find_library(AnalysisTreeBase_LIB NAMES libAnalysisTreeBase.so HINTS ${AnalysisTree_LIBRARY_DIR} REQUIRED)
add_library(AnalysisTreeBase SHARED IMPORTED)
set_target_properties(AnalysisTreeBase PROPERTIES IMPORTED_LOCATION ${AnalysisTreeBase_LIB})

find_library(AnalysisTreeInfra_LIB NAMES libAnalysisTreeInfra.so HINTS ${AnalysisTree_LIBRARY_DIR} REQUIRED)
add_library(AnalysisTreeInfra SHARED IMPORTED)
set_target_properties(AnalysisTreeInfra PROPERTIES IMPORTED_LOCATION ${AnalysisTreeInfra_LIB})

list(APPEND PROJECT_DEPENDENCIES AnalysisTree_Ext)
list(APPEND PROJECT_LINK_DIRECTORIES ${AnalysisTree_LIBRARY_DIR})
list(APPEND PROJECT_INCLUDE_DIRECTORIES ${AnalysisTree_INCLUDE_DIR})
