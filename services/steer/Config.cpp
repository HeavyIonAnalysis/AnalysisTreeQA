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
  const YAML::Node overwrite_node = output["overwrite"];
  overwrite_ = overwrite_node.IsDefined() ? overwrite_node.as<bool>() : false;

  const YAML::Node nevents_node = root["nevents"];
  n_events_ = nevents_node.IsDefined() ? nevents_node.as<long long>() : -1;

  // Assigning an undefined YAML::Node into an already-constructed Node member via operator= (as opposed to fresh-initializing a local one) throws yaml-cpp's InvalidNode; guard with IsDefined() and only assign when
  // there's actually something to assign; the member's own default constructor already leaves it in an equivalent "undefined" state.
  const YAML::Node cuts_node = root["cuts"];
  if (cuts_node.IsDefined()) {
    shared_cuts_node_ = cuts_node;
  }

  const YAML::Node tasks_node = root["tasks"];
  if (tasks_node.IsDefined()) {
    task_nodes_ = tasks_node;
  }
  if (!task_nodes_.IsDefined() || !task_nodes_.IsSequence() || task_nodes_.size() == 0) {
    throw std::runtime_error("Config::LoadYaml(): 'tasks:' must be a non-empty list in " + filename);
  }
}

}// namespace QA
}// namespace AnalysisTree
