#ifndef ANALYSISTREEQA_SERVICES_STEER_TASKS_TRACKQATASK_HPP_
#define ANALYSISTREEQA_SERVICES_STEER_TASKS_TRACKQATASK_HPP_

#include <string>

#include <yaml-cpp/yaml.h>

#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

/// Built-in task wrapping BasicQA.hpp's AddTrackQA(). YAML fields: branch
/// (required), cuts (optional, see CutFactory), name (optional, used as the
/// output subdirectory name).
class TrackQaTask : public Task {
 public:
  explicit TrackQaTask(const YAML::Node& node);
  void Init() override;

 private:
  std::string branch_;
  Cuts* cuts_{nullptr};
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SERVICES_STEER_TASKS_TRACKQATASK_HPP_
