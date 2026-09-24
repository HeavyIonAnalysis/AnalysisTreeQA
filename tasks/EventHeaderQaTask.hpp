#ifndef ANALYSISTREEQA_TASKS_EVENTHEADERQATASK_HPP_
#define ANALYSISTREEQA_TASKS_EVENTHEADERQATASK_HPP_

#include <string>

#include <yaml-cpp/yaml.h>

#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

/// Example task wrapping AddEventHeaderQA() from BasisQA.hpp: see
/// tasks/README.md. YAML fields: branch (required), cuts (optional, see
/// CutFactory), name (optional, used as the output subdirectory name).
class EventHeaderQaTask : public Task {
 public:
  explicit EventHeaderQaTask(const YAML::Node& node);
  void Init() override;

 private:
  std::string branch_;
  Cuts* cuts_{nullptr};
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_TASKS_EVENTHEADERQATASK_HPP_
