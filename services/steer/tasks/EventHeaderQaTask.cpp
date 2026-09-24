#include "EventHeaderQaTask.hpp"

#include "BasicQA.hpp"

#include "CutFactory.hpp"
#include "TaskFactory.hpp"

namespace AnalysisTree {
namespace QA {

EventHeaderQaTask::EventHeaderQaTask(const YAML::Node& node)
    : branch_(node["branch"].as<std::string>()),
      cuts_(CutFactory::Instance().BuildCuts(node["cuts"], node["name"].as<std::string>(std::string("EventHeaderQA")) + "_cuts")) {}

void EventHeaderQaTask::Init() {
  AddEventHeaderQA(this, branch_, cuts_);
  AnalysisTask::Init();
}

ATQA_REGISTER_TASK(EventHeaderQaTask)

}// namespace QA
}// namespace AnalysisTree
