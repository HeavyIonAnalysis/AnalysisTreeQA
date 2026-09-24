#ifndef ANALYSISTREEQA_SERVICES_STEER_TASKS_PARTICLESFLOWQATASK_HPP_
#define ANALYSISTREEQA_SERVICES_STEER_TASKS_PARTICLESFLOWQATASK_HPP_

#include <string>
#include <vector>

#include <yaml-cpp/yaml.h>

#include "AnalysisTree/Field.hpp"

#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

/// Built-in task wrapping BasicQA.hpp's AddParticlesFlowQA(). YAML fields:
/// particles (required, branch name), psi_rp (required, {branch, field}),
/// pdg_codes (required, list of ints), cuts (optional, see CutFactory, ANDed
/// with the per-PDG selection), name (optional, used as the output
/// subdirectory name).
class ParticlesFlowQaTask : public Task {
 public:
  explicit ParticlesFlowQaTask(const YAML::Node& node);
  void Init() override;

 private:
  std::string particles_;
  Field psi_rp_;
  std::vector<int> pdg_codes_;
  Cuts* cuts_{nullptr};
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SERVICES_STEER_TASKS_PARTICLESFLOWQATASK_HPP_
