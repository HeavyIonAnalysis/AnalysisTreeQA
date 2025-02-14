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
    auto weights = this->GetWeights(var_ids.at(0).first);
    int ivw{-1};
    for (auto var : this->GetValues(var_ids.at(0).first)) {
      ++ivw;
      auto weight = weights.at(ivw);
      if(std::fabs(weight) < 1e-6) continue;
      switch (plot.GetNdimentions()) {
        case 1: {
          if(std::fabs(weight - 1) < 1e-4) plot.Fill(var[var_ids.at(0).second]);
          else                                plot.Fill(var[var_ids.at(0).second], weight);
          break;
        }
        case 2: {
          if(std::fabs(weight - 1) < 1e-4) plot.Fill(var[var_ids.at(0).second], var[var_ids.at(1).second]);
          else                                plot.Fill(var[var_ids.at(0).second], var[var_ids.at(1).second], weight);
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
    dir_map_.insert(std::make_pair(dir, MkMultiLevelDir(out_file_, dir)));
  }
  for (auto& entry : entries_) {
    entry.SetOutDir(dir_map_.find(entry.GetDirectoryName())->second);
  }
}

TDirectory* Task::MkMultiLevelDir(TFile* file, const std::string& name) const {
  auto vDirs = splitBySlash(name);
  TDirectory* result;
  for(int iDir=0; iDir<vDirs.size(); iDir++) {
    if(iDir==0) result = MkDirIfNotExists(file, vDirs.at(iDir));
    else result = MkDirIfNotExists(result, vDirs.at(iDir));
  }
  return result;
}

std::vector<std::string> Task::splitBySlash(const std::string& str) {
  std::vector<std::string> result;
  std::stringstream ss(str);
  std::string item;

  // Split the string by slashes
  while (std::getline(ss, item, '/')) {
    result.push_back(item);
  }

  return result;
}


}// namespace QA
}// namespace AnalysisTree
