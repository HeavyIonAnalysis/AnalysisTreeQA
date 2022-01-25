#include "Task.hpp"

namespace AnalysisTree {
namespace QA {

void Task::FillIntegral(EntryConfig& plot){

  double integral_x{0.};
  double integral_y{0.};
  auto var_ids = plot.GetVariablesId();

  for (const auto& var : this->GetValues(var_ids.at(0).first)) {
    integral_x += var[var_ids.at(0).second];
  }
  if(plot.GetNdimentions() > 1) {
    for (const auto& var : this->GetValues(var_ids.at(1).first)) {
      integral_y += var[var_ids.at(1).second];
    }
  }
  if(plot.GetNdimentions() == 1) {
    plot.Fill(integral_x);
  }
  else{
    plot.Fill(integral_x, integral_y);
  }
}

void Task::Exec() {

  AnalysisTask::Exec();

  for (auto& plot : entries_) {

    if(plot.GetType() == EntryConfig::PlotType::kIntegral1D || plot.GetType() == EntryConfig::PlotType::kIntegral2D){
      FillIntegral(plot);
      continue;
    }
    auto var_ids = plot.GetVariablesId();
    for (auto var : this->GetValues(var_ids.at(0).first)) {

      switch (plot.GetNdimentions()) {
        case 1: {
          plot.Fill(var[var_ids.at(0).second]);
          break;
        }
        case 2: {
          plot.Fill(var[var_ids.at(0).second], var[var_ids.at(1).second]);
          break;
        }
      }
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

size_t Task::AddAnalysisEntry(const Axis& a, Cuts* cuts, bool is_integral){
  entries_.emplace_back(EntryConfig(a, cuts, is_integral));
  auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts()));
  entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}});
  return entries_.size() - 1;
}


}// namespace QA
}// namespace AnalysisTree
