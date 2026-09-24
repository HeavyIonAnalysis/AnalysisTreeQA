#include "Config.hpp"

#include <stdexcept>

namespace AnalysisTree {
namespace QA {

void Config::LoadYaml(const std::string& filename) {
  const YAML::Node root = YAML::LoadFile(filename);

  const YAML::Node input = root["input"];
  if (!input.IsDefined()) {
    throw std::runtime_error("Config::LoadYaml(): missing required 'input:' section in " + filename);
  }
  const YAML::Node files = input["files"];
  if (!files.IsDefined() || !files.IsSequence() || files.size() == 0) {
    throw std::runtime_error("Config::LoadYaml(): 'input.files' must be a non-empty list in " + filename);
  }
  input_files_.clear();
  for (const auto& file_node : files) {
    input_files_.push_back(file_node.as<std::string>());
  }
  tree_name_ = input["tree"].as<std::string>();

  const YAML::Node output = root["output"];
  if (!output.IsDefined()) {
    throw std::runtime_error("Config::LoadYaml(): missing required 'output:' section in " + filename);
  }
  output_file_ = output["file"].as<std::string>();
  overwrite_ = output["overwrite"].as<bool>(false);

  n_events_ = root["nevents"].as<long long>(-1);

  shared_cuts_node_ = root["cuts"];

  task_nodes_ = root["tasks"];
  if (!task_nodes_.IsDefined() || !task_nodes_.IsSequence() || task_nodes_.size() == 0) {
    throw std::runtime_error("Config::LoadYaml(): 'tasks:' must be a non-empty list in " + filename);
  }
}

}// namespace QA
}// namespace AnalysisTree
