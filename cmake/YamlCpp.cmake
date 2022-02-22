if (yaml-cpp_BUNDLED)
    message("-- Building bundled yaml-cpp package...")
    include(FetchContent)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

    FetchContent_Declare(
            YAML_CPP
            GIT_REPOSITORY "https://github.com/jbeder/yaml-cpp.git"
            GIT_TAG "yaml-cpp-0.6.3"
            GIT_SHALLOW ON
    )
    set(YAML_CPP_BUILD_TESTS OFF)
    set(YAML_CPP_INSTALL ON)
    FetchContent_MakeAvailable(YAML_CPP)

    list(APPEND PROJECT_INCLUDE_DIRECTORIES ${YAML_CPP_SOURCE_DIR}/include)
else()
    find_package(yaml-cpp REQUIRED)
    list(APPEND PROJECT_INCLUDE_DIRECTORIES ${YAML_CPP_INCLUDE_DIR})
    message(STATUS "YAML_CPP_INCLUDE_DIR: ${YAML_CPP_INCLUDE_DIR}")
endif ()

#list(APPEND PROJECT_LINK_LIBRARIES yaml-cpp)
