#include "ParticleQaTask.hpp"

#include "BasicQA.hpp"

#include "CutFactory.hpp"
#include "TaskFactory.hpp"

namespace AnalysisTree {
namespace QA {

ParticleQaTask::ParticleQaTask(const YAML::Node& node)
    : branch_(node["branch"].as<std::string>()),
      cuts_(CutFactory::Instance().BuildCuts(node["cuts"], node["name"].as<std::string>(std::string("ParticleQA")) + "_cuts")) {}

void ParticleQaTask::Init() {
  AddParticleQA(this, branch_, cuts_);
  AnalysisTask::Init();
}

ATQA_REGISTER_TASK(ParticleQaTask)

}// namespace QA
}// namespace AnalysisTree
