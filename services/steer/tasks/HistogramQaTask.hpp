#ifndef ANALYSISTREEQA_SERVICES_STEER_TASKS_HISTOGRAMQATASK_HPP_
#define ANALYSISTREEQA_SERVICES_STEER_TASKS_HISTOGRAMQATASK_HPP_

#include <yaml-cpp/yaml.h>

#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

/// Fully YAML-declarative task: each entry under its "plots:" list becomes
/// one AddH1/AddH2/AddProfile call. x/y/weight axes are each an arbitrary
/// "branch.field" pair, and cuts are resolved via CutFactory - all exactly
/// as a hand-written macro would call QA::Task itself.
///
/// Cross-branch limit (inherited from AnalysisTree, not an AnalysisTreeQA
/// restriction): AnalysisTree::Matching only ever correlates TWO independent
/// multi-object branches (e.g. VtxTracks <-> TofHits). A plot's x/y/weight
/// axes and its cuts may together reference at most two such branches (plus
/// any number of EventHeader-type branches, which broadcast one value to
/// every entry and need no matching). Referencing a THIRD independent
/// multi-object branch - e.g. VtxTracks.p vs TofHits.mass2 cut on
/// SimParticles.pid - throws a runtime_error from AnalysisTree itself; this
/// is the same limitation the main README's "Known problems" section
/// documents, and fixing it would require a new N-way matching mechanism in
/// the AnalysisTree library, out of scope here. See services/README.md.
///
/// Not covered here (write a private AnalysisTree::Task instead, see
/// tasks/README.md, if needed): integral plots, and anything beyond
/// filling histograms/profiles from per-event branch values.
class HistogramQaTask : public Task {
 public:
  explicit HistogramQaTask(const YAML::Node& node);
  void Init() override;

 private:
  YAML::Node plots_node_;
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SERVICES_STEER_TASKS_HISTOGRAMQATASK_HPP_
