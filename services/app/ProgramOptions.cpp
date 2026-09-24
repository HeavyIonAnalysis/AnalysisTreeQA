#include "ProgramOptions.hpp"

#include <iostream>
#include <stdexcept>

namespace po = boost::program_options;

namespace AnalysisTree {
namespace QA {

ProgramOptions::ProgramOptions(int argc, char** argv) {
  desc_.add_options()("help,h", "print this help message")(
      "print-registered-tasks", po::bool_switch(&print_registered_tasks_),
      "list every task type currently registered (i.e. selectable via 'type:' in YAML) and exit")(
      "config,c", po::value<std::string>(&config_file_)->required(), "YAML config file (see services/README.md)")(
      "input,i", po::value<std::vector<std::string>>(&input_files_)->multitoken(),
      "override the YAML config's input.files")(
      "output,o", po::value<std::string>(&output_file_), "override the YAML config's output.file")(
      "overwrite,w", po::bool_switch(&overwrite_), "override the YAML config's output.overwrite to true");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc_), vm);

  if (vm.count("help")) {
    help_requested_ = true;
    return;
  }
  if (vm.count("print-registered-tasks") && vm["print-registered-tasks"].as<bool>()) {
    print_registered_tasks_ = true;
    return;
  }

  po::notify(vm);
}

void ProgramOptions::PrintHelp(std::ostream& os) const { os << desc_; }

}// namespace QA
}// namespace AnalysisTree
