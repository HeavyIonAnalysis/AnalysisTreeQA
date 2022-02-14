#include <string>

#include "AnalysisTree/TaskManager.hpp"
#include "AnalysisTree/Variable.hpp"

#include "Task.hpp"
#include "YamlReader.hpp"

using namespace AnalysisTree;

void example(const std::string& filelist){
  auto* man = TaskManager::GetInstance();

  auto* task = new QA::Task;
  task->SetOutputFileName("cbm_qa.root");

  Variable pxpy("pxpy", {{"RecTracks", "px"}, {"RecTracks", "py"}}, []( std::vector<double>& var ) { return var.at(0)*var.at(1); });

  task->AddCut(new Cuts("sim_y_cut", {RangeCut("SimParticles.rapidity", 1.5, 3)}));
  task->AddCut(new Cuts("rec_pT_cut", {RangeCut("RecTracks.pT", 1, 1.5)}));
  task->AddVariable(pxpy);

  QA::YamlReader r("/Users/viktor/Soft/AnalysisTreeQA/examples/example.yaml");
  r.AddPlotsFromYaml(task);

  man->AddTask(task);

  man->Init({filelist}, {"tTree"});
  man->Run(-1);
  man->Finish();
}

int main(int argc, char* argv[]){
  if (argc < 2) {
    std::cout << "Error! Please use " << std::endl;
    std::cout << " ./example filename" << std::endl;
    exit(EXIT_FAILURE);
  }

  const std::string filename = argv[1];
  example(filename);

  return 0;
}