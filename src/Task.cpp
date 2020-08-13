#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

void Task::Exec() {
  for (auto& plot : entries_) {
    auto var_ids = plot.GetVariablesId();
    for (auto var : var_manager_->GetValues(var_ids.first)) {
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
  }// plots
}

void Task::Finish() {
  for (auto& plot : entries_) {
    plot.Write();
  }
}

void Task::Init(std::map<std::string, void*>&) {
  std::set<std::string> dirs{};

  for (auto& entry : entries_) {
    dirs.insert(entry.GetDirectoryName());
  }
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
