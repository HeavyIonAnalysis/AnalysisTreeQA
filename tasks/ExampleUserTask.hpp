#ifndef ANALYSISTREEQA_TASKS_EXAMPLEUSERTASK_HPP_
#define ANALYSISTREEQA_TASKS_EXAMPLEUSERTASK_HPP_

#include <string>

#include <yaml-cpp/yaml.h>

#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

/// Worked example for a user-private task: copy this file (.hpp + .cpp),
/// rename the class, and change Init() to book whatever histograms/cuts your
/// analysis needs. Selectable from YAML as soon as it's rebuilt - see
/// tasks/README.md for the full contract.
class ExampleUserTask : public Task {
 public:
  explicit ExampleUserTask(const YAML::Node& node);
  void Init() override;

 private:
  std::string branch_;
  std::string field_;
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_TASKS_EXAMPLEUSERTASK_HPP_
