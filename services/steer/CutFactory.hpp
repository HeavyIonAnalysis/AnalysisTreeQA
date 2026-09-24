#ifndef ANALYSISTREEQA_SERVICES_STEER_CUTFACTORY_HPP_
#define ANALYSISTREEQA_SERVICES_STEER_CUTFACTORY_HPP_

#include <functional>
#include <map>
#include <string>

#include <yaml-cpp/yaml.h>

#include "AnalysisTree/Cuts.hpp"
#include "AnalysisTree/SimpleCut.hpp"

namespace AnalysisTree {
namespace QA {

/// Builds AnalysisTree::Cuts from YAML, either declaratively (range/equals)
/// or via a named, C++-registered custom predicate (for logic that cannot be
/// expressed as a simple range/equals check) - see services/README.md for
/// the full schema and examples.
class CutFactory {
 public:
  /// Builds one AnalysisTree::SimpleCut from a "custom" cut's YAML params.
  using CutCreatorFunction = std::function<AnalysisTree::SimpleCut(const YAML::Node& params)>;

  static CutFactory& Instance();

  /// Called once by Run at startup with the top-level "cuts:" map, so that a
  /// scalar-string cut reference can be resolved wherever it's encountered.
  void SetSharedCutsNode(const YAML::Node& shared_cuts_node);

  /// cuts_field is either: a scalar string (a name looked up in the shared
  /// cuts map set via SetSharedCutsNode), an inline mapping
  /// ({simple_cuts: [...]}), or a missing/null node (-> nullptr). This is
  /// what every task should call for its own "cuts:" YAML field. The
  /// returned Cuts* is heap-allocated and owned by the caller (matching how
  /// AnalysisTree::Cuts* is used throughout the rest of AnalysisTreeQA).
  AnalysisTree::Cuts* BuildCuts(const YAML::Node& cuts_field, const std::string& default_name) const;

  void RegisterCutType(const std::string& type_name, CutCreatorFunction creator);

 private:
  CutFactory() = default;

  AnalysisTree::SimpleCut BuildSimpleCut(const YAML::Node& simple_cut_node) const;
  AnalysisTree::SimpleCut CreateCustomCut(const std::string& type_name, const YAML::Node& params) const;

  YAML::Node shared_cuts_node_;
  std::map<std::string, CutCreatorFunction> cut_creators_;
};

}// namespace QA
}// namespace AnalysisTree

/// Registers a named, C++-defined cut predicate once, at static-init time.
/// Lambda must have signature: AnalysisTree::SimpleCut(const YAML::Node&).
/// Referenced from YAML as: {type: custom, name: TypeName, ...params}.
#define ATQA_REGISTER_CUT(TypeName, Lambda)                                                            \
  namespace {                                                                                          \
  struct TypeName##_CutRegistrar {                                                                     \
    TypeName##_CutRegistrar() { ::AnalysisTree::QA::CutFactory::Instance().RegisterCutType(#TypeName, Lambda); } \
  } TypeName##_cut_registrar_instance;                                                                 \
  }

#endif//ANALYSISTREEQA_SERVICES_STEER_CUTFACTORY_HPP_
