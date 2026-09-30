#ifndef ANALYSISTREEQA_EXAMPLES_EXAMPLEPARTICLETASK_HPP_
#define ANALYSISTREEQA_EXAMPLES_EXAMPLEPARTICLETASK_HPP_

#include <string>

#include <yaml-cpp/yaml.h>

#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

/// Worked example of a services/ task class: books one H1, one H2 and one
/// TProfile directly in Init() (no BasicQA.hpp helper involved, unlike the
/// built-in TrackQaTask), see examples/README.md. YAML fields: branch
/// (required, a Particle-type branch), cuts (optional, see CutFactory),
/// name (optional, used as the output subdirectory name).
class ExampleParticleTask : public Task {
 public:
  explicit ExampleParticleTask(const YAML::Node& node);
  void Init() override;

 private:
  std::string branch_;
  Cuts* cuts_{nullptr};
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_EXAMPLES_EXAMPLEPARTICLETASK_HPP_
