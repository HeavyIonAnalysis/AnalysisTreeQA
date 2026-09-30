#include "ExampleParticleTask.hpp"

#include "AnalysisTree/Variable.hpp"

#include "CutFactory.hpp"
#include "TaskFactory.hpp"

namespace AnalysisTree {
namespace QA {

ExampleParticleTask::ExampleParticleTask(const YAML::Node& node)
    : branch_(node["branch"].as<std::string>()),
      cuts_(CutFactory::Instance().BuildCuts(node["cuts"], GetOrDefault<std::string>(node["name"], "ExampleParticleTask") + "_cuts")) {}

void ExampleParticleTask::Init() {
  // H1: transverse momentum.
  AddH1({"p_{T} (GeV/c)", Variable::FromString(branch_ + ".pT"), {100, 0, 3}}, cuts_);

  // H2: azimuthal angle vs. rapidity.
  AddH2({"#phi", Variable::FromString(branch_ + ".phi"), {100, -3.2, 3.2}},
        {"y_{Lab}", Variable::FromString(branch_ + ".rapidity"), {100, -1, 5}}, cuts_);

  // TProfile: mean pT as a function of rapidity; AddProfile's x-axis sets the binning, its y-axis only sets the expected value range.
  AddProfile("mean_pT_vs_rapidity",
             {"y_{Lab}", Variable::FromString(branch_ + ".rapidity"), {100, -1, 5}},
             {"p_{T} (GeV/c)", Variable::FromString(branch_ + ".pT"), {100, 0, 3}}, cuts_);

  AnalysisTask::Init();
}

ATQA_REGISTER_TASK(ExampleParticleTask)

}// namespace QA
}// namespace AnalysisTree
