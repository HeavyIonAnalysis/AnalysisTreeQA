
#include <string>

#include "AnalysisTree/TaskManager.hpp"
#include "AnalysisTree/Variable.hpp"
#include "Task.hpp"

using namespace AnalysisTree;

void example(const std::string& filelist = "/home/vklochkov/Soft/AnalysisTreeQA/examples/data/filelist_debug.txt"){
  auto* man = TaskManager::GetInstance();

  auto* task = new QA::Task;
  task->SetOutputFileName("cbm_qa.root");

  task->AddH1({"p_{x}, GeV/c", Variable::FromString("VtxTracks.pT"), {100, 0, 3}});

  man->AddTask(task);

  man->Init({filelist}, {"rTree"});
  man->Run(-1);
  man->Finish();
}

int main(int argc, char* argv[]){
  example();
  return 0;
}