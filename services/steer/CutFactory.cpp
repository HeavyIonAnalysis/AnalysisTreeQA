#include "CutFactory.hpp"

#include <stdexcept>

namespace AnalysisTree {
namespace QA {

CutFactory& CutFactory::Instance() {
  static CutFactory instance;
  return instance;
}

void CutFactory::SetSharedCutsNode(const YAML::Node& shared_cuts_node) {
  shared_cuts_node_ = shared_cuts_node;
}

void CutFactory::RegisterCutType(const std::string& type_name, CutCreatorFunction creator) {
  const auto result = cut_creators_.emplace(type_name, std::move(creator));
  if (!result.second) {
    throw std::runtime_error("CutFactory::RegisterCutType(): custom cut type '" + type_name + "' is already registered");
  }
}

AnalysisTree::SimpleCut CutFactory::CreateCustomCut(const std::string& type_name, const YAML::Node& params) const {
  const auto it = cut_creators_.find(type_name);
  if (it == cut_creators_.end()) {
    throw std::runtime_error("CutFactory::CreateCustomCut(): unknown custom cut type '" + type_name
                              + "' (is its .cpp file linked into this binary? see services/README.md)");
  }
  return it->second(params);
}

AnalysisTree::SimpleCut CutFactory::BuildSimpleCut(const YAML::Node& simple_cut_node) const {
  const auto type = simple_cut_node["type"].as<std::string>();
  const auto title = simple_cut_node["title"].as<std::string>("");

  if (type == "range") {
    return RangeCut(simple_cut_node["variable"].as<std::string>(),
                     simple_cut_node["min"].as<double>(),
                     simple_cut_node["max"].as<double>(),
                     title);
  }
  if (type == "equals") {
    return EqualsCut(simple_cut_node["variable"].as<std::string>(), simple_cut_node["value"].as<int>(), title);
  }
  if (type == "custom") {
    return CreateCustomCut(simple_cut_node["name"].as<std::string>(), simple_cut_node);
  }
  throw std::runtime_error("CutFactory::BuildSimpleCut(): unknown simple cut type '" + type
                            + "' (expected 'range', 'equals' or 'custom')");
}

AnalysisTree::Cuts* CutFactory::BuildCuts(const YAML::Node& cuts_field, const std::string& default_name) const {
  if (!cuts_field.IsDefined() || cuts_field.IsNull()) {
    return nullptr;
  }

  if (cuts_field.IsScalar()) {
    const auto name = cuts_field.as<std::string>();
    const YAML::Node shared_entry = shared_cuts_node_[name];
    if (!shared_entry.IsDefined()) {
      throw std::runtime_error("CutFactory::BuildCuts(): no shared cut named '" + name
                                + "' found under the top-level 'cuts:' map");
    }
    return BuildCuts(shared_entry, name);
  }

  const YAML::Node simple_cuts = cuts_field["simple_cuts"];
  if (!simple_cuts.IsDefined() || !simple_cuts.IsSequence()) {
    throw std::runtime_error("CutFactory::BuildCuts(): expected a 'simple_cuts' list while building cut '"
                              + default_name + "'");
  }

  std::vector<AnalysisTree::SimpleCut> cuts;
  cuts.reserve(simple_cuts.size());
  for (const auto& simple_cut_node : simple_cuts) {
    cuts.emplace_back(BuildSimpleCut(simple_cut_node));
  }
  return new AnalysisTree::Cuts(default_name, cuts);
}

}// namespace QA
}// namespace AnalysisTree
