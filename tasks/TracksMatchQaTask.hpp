#ifndef ANALYSISTREEQA_TASKS_TRACKSMATCHQATASK_HPP_
#define ANALYSISTREEQA_TASKS_TRACKSMATCHQATASK_HPP_

#include <string>

#include <yaml-cpp/yaml.h>

#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

/// Example task wrapping AddTracksMatchQA() from BasisQA.hpp: see
/// tasks/README.md. YAML fields: rec_branch, sim_branch (both required),
/// cuts (optional, see CutFactory), name (optional, used as the output
/// subdirectory name).
class TracksMatchQaTask : public Task {
 public:
  explicit TracksMatchQaTask(const YAML::Node& node);
  void Init() override;

 private:
  std::string rec_branch_;
  std::string sim_branch_;
  Cuts* cuts_{nullptr};
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_TASKS_TRACKSMATCHQATASK_HPP_
