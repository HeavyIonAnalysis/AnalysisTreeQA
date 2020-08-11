
set(PlotsMaker_INSTALL_DIR ${EXTERNAL_INSTALL_DIR}/)
set(PlotsMaker_INCLUDE_DIR ${PlotsMaker_INSTALL_DIR}/include)
set(PlotsMaker_LIBRARY_DIR ${PlotsMaker_INSTALL_DIR}/lib)

ExternalProject_Add(PlotsMaker_Ext
        GIT_REPOSITORY  "https://git.cbm.gsi.de/pwg-c2f/utils/plots_maker.git"
        GIT_TAG         "master"
        UPDATE_DISCONNECTED ${UPDATE_DISCONNECTED}
        SOURCE_DIR      "${EXTERNAL_DIR}/PlotsMaker_src"
        BINARY_DIR      "${EXTERNAL_DIR}/PlotsMaker_build"
        INSTALL_DIR     "${PlotsMaker_INSTALL_DIR}"
        CMAKE_ARGS
        "-DCMAKE_INSTALL_PREFIX=${PlotsMaker_INSTALL_DIR}"
        "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
        )

list(APPEND PROJECT_DEPENDENCIES PlotsMaker_Ext)
list(APPEND PROJECT_LINK_DIRECTORIES ${PlotsMaker_LIBRARY_DIR})
list(APPEND PROJECT_INCLUDE_DIRECTORIES ${PlotsMaker_INCLUDE_DIR})
