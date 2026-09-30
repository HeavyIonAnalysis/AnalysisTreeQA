#ifndef ANALYSISTREEQA_SERVICES_APP_APPLICATION_HPP_
#define ANALYSISTREEQA_SERVICES_APP_APPLICATION_HPP_

#include "ProgramOptions.hpp"

namespace AnalysisTree {
namespace QA {

/// Glue between the parsed command line and the Run driver: validates paths,
/// loads the YAML config, applies any CLI overrides, then runs.
class Application {
 public:
  explicit Application(ProgramOptions options);

  void Exec();

 private:
  ProgramOptions options_;
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SERVICES_APP_APPLICATION_HPP_
