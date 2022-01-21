#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

void Task::Exec() {

  AnalysisTask::Exec();

  double integral{0.};

  for (auto& plot : entries_) {
    auto var_ids = plot.GetVariablesId();
    for (auto var : this->GetValues(var_ids.first)) {
      if(plot.GetType() == EntryConfig::PlotType::kIntergral){
        integral += var[var_ids.second.at(0)];
      }
      else{
        switch (plot.GetNdimentions()) {
          case 1: {
            plot.Fill(var[var_ids.second.at(0)]);
            break;
          }
          case 2: {
            plot.Fill(var[var_ids.second.at(0)], var[var_ids.second.at(1)]);
            break;
          }
        }
      }
    }

    if(plot.GetType() == EntryConfig::PlotType::kIntergral){
      plot.Fill(integral);
    }
  }// plots
}

void Task::Finish() {
  out_file_->cd();
  for (auto& plot : entries_) {
    plot.Write();
  }
  out_file_->Close();
}

void Task::Init() {

  AnalysisTask::Init();
  std::set<std::string> dirs{};

  for (auto& entry : entries_) {
    dirs.insert(entry.GetDirectoryName());
  }
  out_file_ = new TFile(out_file_name_.c_str(), "recreate");
  for (const auto& dir : dirs) {
    out_file_->cd();
    dir_map_.insert(std::make_pair(dir, out_file_->mkdir(dir.c_str())));
  }
  for (auto& entry : entries_) {
    entry.SetOutDir(dir_map_.find(entry.GetDirectoryName())->second);
  }
}

}// namespace QA
}// namespace AnalysisTree
