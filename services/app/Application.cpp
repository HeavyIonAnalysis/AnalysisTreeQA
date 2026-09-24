#include "Application.hpp"

#include <filesystem>
#include <stdexcept>
#include <utility>

#include "Config.hpp"
#include "Run.hpp"

namespace fs = std::filesystem;

namespace AnalysisTree {
namespace QA {

Application::Application(ProgramOptions options) : options_(std::move(options)) {}

void Application::Exec() {
  const fs::path config_path = fs::absolute(options_.ConfigFile());
  if (!fs::exists(config_path)) {
    throw std::runtime_error("Config file does not exist: " + config_path.string());
  }

  Config config;
  config.LoadYaml(config_path.string());

  if (options_.HasInputFiles()) {
    config.SetInputFiles(options_.InputFiles());
  }
  for (const auto& input_file : config.InputFiles()) {
    if (!fs::exists(fs::absolute(input_file))) {
      throw std::runtime_error("Input file does not exist: " + input_file);
    }
  }

  if (options_.HasOutputFile()) {
    config.SetOutputFile(options_.OutputFile());
  }
  if (options_.Overwrite()) {
    config.SetOverwrite(true);
  }
  if (fs::exists(fs::absolute(config.OutputFile())) && !config.Overwrite()) {
    throw std::runtime_error("Output file already exists (pass --overwrite to replace it): " + config.OutputFile());
  }

  Run run(std::move(config));
  run.Exec();
}

}// namespace QA
}// namespace AnalysisTree
