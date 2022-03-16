if (AnalysisTreeQA_BUILD_TESTS)
    enable_testing()
    include(GoogleTest)
    # Download and unpack googletest at configure time
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-GTest.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/googletest-download )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            RESULT_VARIABLE result
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/test/googletest-download )
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
            ${CMAKE_BINARY_DIR}/googletest-build
            EXCLUDE_FROM_ALL)

    add_executable(AnalysisTreeQA_UnitTests test_runner.cxx)
    include_directories(${PROJECT_INCLUDE_DIRECTORIES})
    target_link_libraries(AnalysisTreeQA_UnitTests PRIVATE AnalysisTreeQA gtest_main)
    gtest_discover_tests(AnalysisTreeQA_UnitTests TEST_PREFIX AnalysisTreeQA_)

endif() # AnalysisTree_BUILD_TESTS
