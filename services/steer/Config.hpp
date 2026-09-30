#ifndef ANALYSISTREEQA_SERVICES_STEER_CONFIG_HPP_
#define ANALYSISTREEQA_SERVICES_STEER_CONFIG_HPP_

#include <string>
#include <utility>
#include <vector>

#include <yaml-cpp/yaml.h>

namespace AnalysisTree {
namespace QA {

/// Holds the parsed content of one AnalysisTreeQA YAML config file. See
/// services/README.md for the full schema (input/output/cuts/tasks).
class Config {
 public:
  void LoadYaml(const std::string& filename);

  const std::vector<std::string>& InputFiles() const { return input_files_; }
  const std::string& TreeName() const { return tree_name_; }
  const std::string& OutputFile() const { return output_file_; }
  bool Overwrite() const { return overwrite_; }
  long long NEvents() const { return n_events_; }

  /// Let the CLI override individual YAML-loaded settings (see
  /// services/app/ProgramOptions.hpp) - used only if the corresponding flag
  /// was actually given on the command line.
  void SetInputFiles(std::vector<std::string> files) { input_files_ = std::move(files); }
  void SetOutputFile(std::string file) { output_file_ = std::move(file); }
  void SetOverwrite(bool overwrite) { overwrite_ = overwrite; }

  /// The top-level "cuts:" map, handed to CutFactory::SetSharedCutsNode().
  const YAML::Node& SharedCutsNode() const { return shared_cuts_node_; }
  /// The "tasks:" list, one YAML::Node per task to construct.
  const YAML::Node& TaskNodes() const { return task_nodes_; }

 private:
  std::vector<std::string> input_files_;
  std::string tree_name_;
  std::string output_file_;
  bool overwrite_{false};
  long long n_events_{-1};
  YAML::Node shared_cuts_node_;
  YAML::Node task_nodes_;
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SERVICES_STEER_CONFIG_HPP_
