#ifndef ANALYSISTREEQA_SERVICES_APP_PROGRAMOPTIONS_HPP_
#define ANALYSISTREEQA_SERVICES_APP_PROGRAMOPTIONS_HPP_

#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace AnalysisTree {
namespace QA {

/// Parses the analysistreeqa command line. -c/--config is the only
/// required flag; -i/--input and -o/--output/-w/--overwrite are optional
/// convenience overrides for the corresponding YAML settings (see
/// services/README.md), so a quick rerun against a different file doesn't
/// require editing the config.
class ProgramOptions {
 public:
  ProgramOptions(int argc, char** argv);

  bool HelpRequested() const { return help_requested_; }
  void PrintHelp(std::ostream& os) const;

  /// Like --help: bypasses the --config requirement, since the point is
  /// discovering task types before writing a config at all.
  bool PrintRegisteredTasksRequested() const { return print_registered_tasks_; }

  const std::string& ConfigFile() const { return config_file_; }

  bool HasInputFiles() const { return !input_files_.empty(); }
  const std::vector<std::string>& InputFiles() const { return input_files_; }

  bool HasOutputFile() const { return !output_file_.empty(); }
  const std::string& OutputFile() const { return output_file_; }

  bool Overwrite() const { return overwrite_; }

 private:
  boost::program_options::options_description desc_{"analysistreeqa options"};
  bool help_requested_{false};
  bool print_registered_tasks_{false};
  std::string config_file_;
  std::vector<std::string> input_files_;
  std::string output_file_;
  bool overwrite_{false};
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SERVICES_APP_PROGRAMOPTIONS_HPP_
