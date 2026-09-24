#include "TracksMatchQaTask.hpp"

#include "BasicQA.hpp"

#include "CutFactory.hpp"
#include "TaskFactory.hpp"

namespace AnalysisTree {
namespace QA {

TracksMatchQaTask::TracksMatchQaTask(const YAML::Node& node)
    : rec_branch_(node["rec_branch"].as<std::string>()),
      sim_branch_(node["sim_branch"].as<std::string>()),
      cuts_(CutFactory::Instance().BuildCuts(node["cuts"], node["name"].as<std::string>(std::string("TracksMatchQA")) + "_cuts")) {}

void TracksMatchQaTask::Init() {
  AddTracksMatchQA(this, rec_branch_, sim_branch_, cuts_);
  AnalysisTask::Init();
}

ATQA_REGISTER_TASK(TracksMatchQaTask)

}// namespace QA
}// namespace AnalysisTree
