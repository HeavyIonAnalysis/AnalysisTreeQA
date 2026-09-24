#include "Run.hpp"

#include <utility>

#include <TFile.h>

#include "AnalysisTree/TaskManager.hpp"

#include "Task.hpp"

#include "CutFactory.hpp"
#include "TaskFactory.hpp"

namespace AnalysisTree {
namespace QA {

Run::Run(Config config) : config_(std::move(config)) {}

void Run::Exec() {
  auto* out_file = new TFile(config_.OutputFile().c_str(), config_.Overwrite() ? "recreate" : "create");

  CutFactory::Instance().SetSharedCutsNode(config_.SharedCutsNode());

  auto* man = AnalysisTree::TaskManager::GetInstance();

  for (const auto& entry : config_.TaskNodes()) {
    const auto type_name = entry["type"].as<std::string>();
    const auto task_name = entry["name"].as<std::string>(type_name);

    auto* task = TaskFactory::Instance().CreateTask(type_name, entry);

    if (auto* qa_task = dynamic_cast<AnalysisTree::QA::Task*>(task)) {
      qa_task->AttachOutputFile(out_file);
      qa_task->SetTopLevelDirName(task_name);
    }

    man->AddTask(task);
  }

  man->Init(config_.InputFiles(), {config_.TreeName()});
  man->Run(config_.NEvents());
  man->Finish();

  out_file->Write();
  out_file->Close();
}

}// namespace QA
}// namespace AnalysisTree
