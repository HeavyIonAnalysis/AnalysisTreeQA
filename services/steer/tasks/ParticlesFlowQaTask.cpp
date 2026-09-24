#include "ParticlesFlowQaTask.hpp"

#include "BasicQA.hpp"

#include "CutFactory.hpp"
#include "TaskFactory.hpp"

namespace AnalysisTree {
namespace QA {

ParticlesFlowQaTask::ParticlesFlowQaTask(const YAML::Node& node)
    : particles_(node["particles"].as<std::string>()),
      psi_rp_(node["psi_rp"]["branch"].as<std::string>(), node["psi_rp"]["field"].as<std::string>()),
      cuts_(CutFactory::Instance().BuildCuts(node["cuts"], node["name"].as<std::string>(std::string("ParticlesFlowQA")) + "_cuts")) {
  for (const auto& pdg_node : node["pdg_codes"]) {
    pdg_codes_.push_back(pdg_node.as<int>());
  }
}

void ParticlesFlowQaTask::Init() {
  AddParticlesFlowQA(this, particles_, psi_rp_, pdg_codes_, cuts_);
  AnalysisTask::Init();
}

ATQA_REGISTER_TASK(ParticlesFlowQaTask)

}// namespace QA
}// namespace AnalysisTree
