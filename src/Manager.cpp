#include "Manager.hpp"
#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

void Manager::AddTask(QA::Task* task) {
  assert(tasks_.empty());// For the moment one task per Manager

  auto* var_task = new VarManager();

  for (auto& entry : task->Entries()) {
    if (entry.Axes().size() > 2 || entry.Axes().empty()) {
      throw std::runtime_error("entry.Axes().size() is more than 2 or 0");
    }
    auto var_id = var_task->AddEntry(VarManagerEntry(entry.GetVariables(), entry.GetEntryCuts()));
    entry.SetVariablesId(var_id);
  }
  var_task->FillBranchNames();

  var_task->SetCutsMap(cuts_map_);
  task->SetPointerToVarManager(var_task);

  tasks_.emplace_back(var_task);//Needs to be executed first
  tasks_.emplace_back(task);
}
}// namespace QA
}// namespace AnalysisTree
