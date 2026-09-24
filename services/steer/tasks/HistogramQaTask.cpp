#include "HistogramQaTask.hpp"

#include <stdexcept>
#include <string>

#include "AnalysisTree/Variable.hpp"

#include "CutFactory.hpp"
#include "TaskFactory.hpp"

namespace AnalysisTree {
namespace QA {

namespace {

Axis BuildAxis(const YAML::Node& axis_node) {
  const auto branch = axis_node["branch"].as<std::string>();
  const auto field = axis_node["field"].as<std::string>();
  const YAML::Node bins = axis_node["bins"];
  const int nbins = bins["nbins"].as<int>();
  const double min = bins["min"].as<double>();
  const double max = bins["max"].as<double>();
  const auto title = axis_node["title"].as<std::string>(branch + "." + field);
  return Axis(title, Variable::FromString(branch + "." + field), TAxis(nbins, min, max));
}

Variable BuildWeight(const YAML::Node& plot_node) {
  const YAML::Node weight_node = plot_node["weight"];
  if (!weight_node.IsDefined()) {
    return Variable{};
  }
  return Variable::FromString(weight_node["branch"].as<std::string>() + "." + weight_node["field"].as<std::string>());
}

}// namespace

HistogramQaTask::HistogramQaTask(const YAML::Node& node) : plots_node_(node["plots"]) {
  if (!plots_node_.IsDefined() || !plots_node_.IsSequence() || plots_node_.size() == 0) {
    throw std::runtime_error("HistogramQaTask: expected a non-empty 'plots:' list");
  }
}

void HistogramQaTask::Init() {
  size_t plot_index{0};
  for (const auto& plot_node : plots_node_) {
    const auto kind = plot_node["kind"].as<std::string>();
    const auto name = plot_node["name"].as<std::string>("");
    const auto cuts_default_name = name.empty() ? ("plot_" + std::to_string(plot_index) + "_cuts") : (name + "_cuts");
    auto* cuts = CutFactory::Instance().BuildCuts(plot_node["cuts"], cuts_default_name);
    const auto weight = BuildWeight(plot_node);

    if (kind == "h1") {
      AddH1(name, BuildAxis(plot_node["x"]), cuts, weight);
    } else if (kind == "h2") {
      AddH2(name, BuildAxis(plot_node["x"]), BuildAxis(plot_node["y"]), cuts, weight);
    } else if (kind == "profile") {
      AddProfile(name, BuildAxis(plot_node["x"]), BuildAxis(plot_node["y"]), cuts, weight);
    } else {
      throw std::runtime_error("HistogramQaTask: unknown plot kind '" + kind + "' (expected 'h1', 'h2' or 'profile')");
    }
    ++plot_index;
  }
  AnalysisTask::Init();
}

ATQA_REGISTER_TASK(HistogramQaTask)

}// namespace QA
}// namespace AnalysisTree
