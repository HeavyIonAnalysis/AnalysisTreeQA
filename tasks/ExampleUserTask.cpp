#include "ExampleUserTask.hpp"

#include "AnalysisTree/Variable.hpp"

#include "TaskFactory.hpp"

namespace AnalysisTree {
namespace QA {

// YAML fields: branch, field (both required) - books one 1D histogram of
// "branch.field". Replace this with whatever your own analysis needs.
ExampleUserTask::ExampleUserTask(const YAML::Node& node)
    : branch_(node["branch"].as<std::string>()),
      field_(node["field"].as<std::string>()) {}

void ExampleUserTask::Init() {
  Axis axis(field_, Variable::FromString(branch_ + "." + field_), TAxis(100, 0, 100));
  AddH1(field_, axis);
  AnalysisTask::Init();
}

ATQA_REGISTER_TASK(ExampleUserTask)

}// namespace QA
}// namespace AnalysisTree
