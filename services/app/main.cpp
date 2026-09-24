#include <cstdlib>
#include <iostream>

#include "Application.hpp"
#include "ProgramOptions.hpp"
#include "TaskFactory.hpp"

int main(int argc, char* argv[]) {
  try {
    AnalysisTree::QA::ProgramOptions options(argc, argv);
    if (options.HelpRequested()) {
      options.PrintHelp(std::cout);
      return EXIT_SUCCESS;
    }
    if (options.PrintRegisteredTasksRequested()) {
      std::cout << "Registered task types (usable as 'type:' in YAML):\n";
      for (const auto& type_name : AnalysisTree::QA::TaskFactory::Instance().RegisteredTypeNames()) {
        std::cout << "  " << type_name << "\n";
      }
      return EXIT_SUCCESS;
    }

    AnalysisTree::QA::Application app(options);
    app.Exec();
  } catch (const std::exception& e) {
    std::cerr << "analysistreeqa: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
