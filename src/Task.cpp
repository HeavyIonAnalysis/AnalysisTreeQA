#include "Task.hpp"

#include <utility>

namespace AnalysisTree::QA {

size_t Task::AddH1(const std::string& name, const Axis& x, Cuts* cuts, Variable weight) {
  CreateOutputFileIfNotYet();
  weight.IfEmptyVariableConvertToOnes(x);
  entries_.emplace_back(x, weight, name, cuts, false);
  const std::string dirName = ConstructOutputDirectoryName();
  TDirectory* dir = MkMultiLevelDir(out_file_, dirName);
  ANALYSISTREE_UTILS_VISIT(setdirectory_struct(dir), entries_.back().GetPlot());
  ANALYSISTREE_UTILS_VISIT(setname_struct(entries_.back().GetName()), entries_.back().GetPlot());
  auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts(), entries_.back().GetVariableForWeight()));
  entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}});
  return entries_.size() - 1;
}

size_t Task::AddH1(const Axis& x, Cuts* cuts, Variable weight) {
  return AddH1("", x, cuts, std::move(weight));
}

size_t Task::AddH2(const std::string& name, const Axis& x, const Axis& y, Cuts* cuts, Variable weight) {
  CreateOutputFileIfNotYet();
  weight.IfEmptyVariableConvertToOnes(x);
  entries_.emplace_back(x, y, weight, name, cuts);
  const std::string dirName = ConstructOutputDirectoryName();
  TDirectory* dir = MkMultiLevelDir(out_file_, dirName);
  ANALYSISTREE_UTILS_VISIT(setdirectory_struct(dir), entries_.back().GetPlot());
  ANALYSISTREE_UTILS_VISIT(setname_struct(entries_.back().GetName()), entries_.back().GetPlot());
  auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts(), entries_.back().GetVariableForWeight()));
  entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}, {var_id.first, var_id.second.at(1)}});
  return entries_.size() - 1;
}

size_t Task::AddH2(const Axis& x, const Axis& y, Cuts* cuts, Variable weight) {
  return AddH2("", x, y, cuts, std::move(weight));
}

size_t Task::AddProfile(const std::string& name, const Axis& x, const Axis& y, Cuts* cuts, Variable weight) {
  CreateOutputFileIfNotYet();
  weight.IfEmptyVariableConvertToOnes(x);
  entries_.emplace_back(x, y, weight, name, cuts, true);
  const std::string dirName = ConstructOutputDirectoryName();
  TDirectory* dir = MkMultiLevelDir(out_file_, dirName);
  ANALYSISTREE_UTILS_VISIT(setdirectory_struct(dir), entries_.back().GetPlot());
  ANALYSISTREE_UTILS_VISIT(setname_struct(entries_.back().GetName()), entries_.back().GetPlot());
  auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts(), entries_.back().GetVariableForWeight()));
  entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}, {var_id.first, var_id.second.at(1)}});
  return entries_.size() - 1;
}

size_t Task::AddProfile(const Axis& x, const Axis& y, Cuts* cuts, Variable weight) {
  return AddProfile("", x, y, cuts, std::move(weight));
}

size_t Task::AddIntegral(const std::string& name, const Axis& x, Cuts* cuts, Variable weight) {
  CreateOutputFileIfNotYet();
  weight.IfEmptyVariableConvertToOnes(x);
  entries_.emplace_back(x, weight, name, cuts, true);
  const std::string dirName = ConstructOutputDirectoryName();
  TDirectory* dir = MkMultiLevelDir(out_file_, dirName);
  ANALYSISTREE_UTILS_VISIT(setdirectory_struct(dir), entries_.back().GetPlot());
  ANALYSISTREE_UTILS_VISIT(setname_struct(entries_.back().GetName()), entries_.back().GetPlot());
  auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts(), entries_.back().GetVariableForWeight()));
  entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}});
  return entries_.size() - 1;
}

size_t Task::AddIntegral(const Axis& x, Cuts* cuts, Variable weight) {
  return AddIntegral("", x, cuts, std::move(weight));
}

size_t Task::AddIntegral(const Axis& x, const Axis& y, Cuts* cuts_x, Cuts* cuts_y) {
  CreateOutputFileIfNotYet();
  entries_.emplace_back(x, cuts_x, y, cuts_y);
  const std::string dirName = ConstructOutputDirectoryName();
  TDirectory* dir = MkMultiLevelDir(out_file_, dirName);
  ANALYSISTREE_UTILS_VISIT(setdirectory_struct(dir), entries_.back().GetPlot());
  ANALYSISTREE_UTILS_VISIT(setname_struct(entries_.back().GetName()), entries_.back().GetPlot());
  auto var_id_x = AddEntry(AnalysisEntry({entries_.back().GetVariables()[0]}, cuts_x));
  auto var_id_y = AddEntry(AnalysisEntry({entries_.back().GetVariables()[1]}, cuts_y));
  entries_.back().SetVariablesId({{var_id_x.first, var_id_x.second.at(0)}, {var_id_y.first, var_id_y.second.at(0)}});
  return entries_.size() - 1;
}

void Task::FillIntegral(EntryConfig& plot) {
  double integral_x{0.};
  double integral_y{0.};
  auto var_ids = plot.GetVariablesId();

  for (const auto& var : this->GetValues(var_ids.at(0).first)) {
    integral_x += var[var_ids.at(0).second];
  }
  if (plot.GetNdimensions() > 1) {
    for (const auto& var : this->GetValues(var_ids.at(1).first)) {
      integral_y += var[var_ids.at(1).second];
    }
  }
  if (plot.GetNdimensions() == 1) {
    plot.Fill(integral_x);
  } else {
    plot.Fill(integral_x, integral_y);
  }
}

void Task::Exec() {
  AnalysisTask::Exec();

  for (auto& plot : entries_) {

    if (plot.GetType() == EntryConfig::PlotType::kIntegral1D || plot.GetType() == EntryConfig::PlotType::kIntegral2D) {
      FillIntegral(plot);
      continue;
    }
    auto var_ids = plot.GetVariablesId();
    auto weights = this->GetWeights(var_ids.at(0).first);
    int ivw{-1};
    for (auto var : this->GetValues(var_ids.at(0).first)) {
      ++ivw;
      auto weight = weights.at(ivw);
      if (std::fabs(weight) < 1e-6) continue;
      switch (plot.GetNdimensions()) {
        case 1: {
          if (std::fabs(weight - 1) < 1e-4) plot.Fill(var[var_ids.at(0).second]);
          else
            plot.Fill(var[var_ids.at(0).second], weight);
          break;
        }
        case 2: {
          if (std::fabs(weight - 1) < 1e-4) plot.Fill(var[var_ids.at(0).second], var[var_ids.at(1).second]);
          else
            plot.Fill(var[var_ids.at(0).second], var[var_ids.at(1).second], weight);
          break;
        }
      }
    }
  }// plots
}

void Task::Finish() {
  out_file_->Write();
  out_file_->Close();
}

TDirectory* Task::MkMultiLevelDir(TFile* file, const std::string& name) const {
  auto splitBySlash = [](const std::string& str) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    // Split the string by slashes
    while (std::getline(ss, item, '/')) {
      result.push_back(item);
    }

    return result;
  };

  auto vDirs = splitBySlash(name);
  TDirectory* result;
  for (int iDir = 0; iDir < vDirs.size(); iDir++) {
    if (iDir == 0) result = MkDirIfNotExists(file, vDirs.at(iDir));
    else
      result = MkDirIfNotExists(result, vDirs.at(iDir));
  }
  return result;
}

void Task::CreateOutputFileIfNotYet() {
  if (out_file_ == nullptr) out_file_ = new TFile(out_file_name_.c_str(), out_file_option_.c_str());
}

std::string Task::ConstructOutputDirectoryName() {
  const std::string entryName = entries_.back().GetDirectoryName();
  std::string dirName = toplevel_dir_name_.empty() ? entryName : toplevel_dir_name_;
  if (is_append_dir_name_with_entry_name_ && !toplevel_dir_name_.empty()) dirName.append("/" + entryName);

  return dirName;
}

}// namespace AnalysisTree::QA
