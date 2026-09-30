#include "TrackQaTask.hpp"

#include "BasicQA.hpp"

#include "CutFactory.hpp"
#include "TaskFactory.hpp"

namespace AnalysisTree {
namespace QA {

TrackQaTask::TrackQaTask(const YAML::Node& node)
    : branch_(node["branch"].as<std::string>()),
      cuts_(CutFactory::Instance().BuildCuts(node["cuts"], GetOrDefault<std::string>(node["name"], "TrackQA") + "_cuts")) {}

void TrackQaTask::Init() {
  AddTrackQA(this, branch_, cuts_);
  AnalysisTask::Init();
}

ATQA_REGISTER_TASK(TrackQaTask)

}// namespace QA
}// namespace AnalysisTree
