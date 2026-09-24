add_subdirectory(steer)
add_subdirectory(app)

install(DIRECTORY config/ DESTINATION share/AnalysisTreeQA/config FILES_MATCHING PATTERN "*.yaml")
