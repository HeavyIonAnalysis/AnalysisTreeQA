#include "ExampleEventHeaderTask.hpp"

#include "AnalysisTree/Variable.hpp"

#include "CutFactory.hpp"
#include "TaskFactory.hpp"

namespace AnalysisTree {
namespace QA {

ExampleEventHeaderTask::ExampleEventHeaderTask(const YAML::Node& node)
    : branch_(node["branch"].as<std::string>()),
      cuts_(CutFactory::Instance().BuildCuts(node["cuts"], GetOrDefault<std::string>(node["name"], "ExampleEventHeaderTask") + "_cuts")) {}

void ExampleEventHeaderTask::Init() {
  // H1: primary vertex z position.
  AddH1({"z_{vertex} (cm)", Variable::FromString(branch_ + ".vtx_z"), {100, -1, 1}}, cuts_);

  // H2: primary vertex transverse position.
  AddH2({"x_{vertex} (cm)", Variable::FromString(branch_ + ".vtx_x"), {100, -1, 1}},
        {"y_{vertex} (cm)", Variable::FromString(branch_ + ".vtx_y"), {100, -1, 1}}, cuts_);

  // TProfile: mean vertex z position as a function of vertex x; AddProfile's x-axis sets the binning, its y-axis only sets the expected value range.
  AddProfile("vtx_z_vs_vtx_x_profile",
             {"x_{vertex} (cm)", Variable::FromString(branch_ + ".vtx_x"), {100, -1, 1}},
             {"z_{vertex} (cm)", Variable::FromString(branch_ + ".vtx_z"), {100, -1, 1}}, cuts_);

  AnalysisTask::Init();
}

ATQA_REGISTER_TASK(ExampleEventHeaderTask)

}// namespace QA
}// namespace AnalysisTree
