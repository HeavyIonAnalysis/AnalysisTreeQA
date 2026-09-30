#ifndef ANALYSISTREEQA_SERVICES_STEER_RUN_HPP_
#define ANALYSISTREEQA_SERVICES_STEER_RUN_HPP_

#include "Config.hpp"

namespace AnalysisTree {
namespace QA {

/// Orchestrates one QA run: builds every task listed under the config's
/// "tasks:" list via TaskFactory, wires the shared output file into every
/// AnalysisTree::QA::Task subclass among them, and drives
/// AnalysisTree::TaskManager end to end.
class Run {
 public:
  explicit Run(Config config);

  void Exec();

 private:
  Config config_;
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SERVICES_STEER_RUN_HPP_
