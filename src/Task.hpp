#ifndef ANALYSISTREEQA_SRC_TASK_HPP_
#define ANALYSISTREEQA_SRC_TASK_HPP_

#include <utility>

#include "TFile.h"

#include "AnalysisTree/AnalysisTask.hpp"

#include "EntryConfig.hpp"

namespace AnalysisTree {
namespace QA {

struct setdirectory_struct : Utils::Visitor<void> {
  explicit setdirectory_struct(TDirectory* dir) : dir_(dir) {}
  template<class PlotType>
  void operator()(PlotType* p) const { p->SetDirectory(dir_); }
  TDirectory* dir_;
};

struct setname_struct : Utils::Visitor<void> {
  explicit setname_struct(const std::string& name) : name_(name) {}
  template<class PlotType>
  void operator()(PlotType* p) const { p->SetName(name_.c_str()); }
  std::string name_;
};

class Task : public AnalysisTask {
 public:
  Task() = default;

  void Init() override;
  void Exec() override;
  void Finish() override;

  size_t AddH1(const std::string& name, const Axis& x, Cuts* cuts = nullptr, Variable weight = Variable{}) {
    CreateOutputFileIfNotYet();
    weight.IfEmptyVariableConvertToOnes(x);
    entries_.emplace_back(EntryConfig(x, weight, name, cuts, false));
    TDirectory* dir = MkMultiLevelDir(out_file_, toplevel_dir_name_ + "/" + entries_.back().GetDirectoryName());
    ANALYSISTREE_UTILS_VISIT(setdirectory_struct(dir), entries_.back().GetPlot());
    ANALYSISTREE_UTILS_VISIT(setname_struct(entries_.back().GetName()), entries_.back().GetPlot());
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts(), entries_.back().GetVariableForWeight()));
    entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}});
    return entries_.size() - 1;
  }

  size_t AddH1(const Axis& x, Cuts* cuts = nullptr, Variable weight = Variable{}) {
    return AddH1("", x, cuts, weight);
  }

  size_t AddH2(const std::string& name, const Axis& x, const Axis& y, Cuts* cuts = nullptr, Variable weight = Variable{}) {
    CreateOutputFileIfNotYet();
    weight.IfEmptyVariableConvertToOnes(x);
    entries_.emplace_back(EntryConfig(x, y, weight, name, cuts));
    TDirectory* dir = MkMultiLevelDir(out_file_, toplevel_dir_name_ + "/" + entries_.back().GetDirectoryName());
    ANALYSISTREE_UTILS_VISIT(setdirectory_struct(dir), entries_.back().GetPlot());
    ANALYSISTREE_UTILS_VISIT(setname_struct(entries_.back().GetName()), entries_.back().GetPlot());
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts(), entries_.back().GetVariableForWeight()));
    entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}, {var_id.first, var_id.second.at(1)}});
    return entries_.size() - 1;
  }

  size_t AddH2(const Axis& x, const Axis& y, Cuts* cuts = nullptr, Variable weight = Variable{}) {
    return AddH2("", x, y, cuts, weight);
  }

  size_t AddProfile(const std::string& name, const Axis& x, const Axis& y, Cuts* cuts = nullptr, Variable weight = Variable{}) {
    CreateOutputFileIfNotYet();
    weight.IfEmptyVariableConvertToOnes(x);
    entries_.emplace_back(EntryConfig(x, y, weight, name, cuts, true));
    TDirectory* dir = MkMultiLevelDir(out_file_, toplevel_dir_name_ + "/" + entries_.back().GetDirectoryName());
    ANALYSISTREE_UTILS_VISIT(setdirectory_struct(dir), entries_.back().GetPlot());
    ANALYSISTREE_UTILS_VISIT(setname_struct(entries_.back().GetName()), entries_.back().GetPlot());
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts(), entries_.back().GetVariableForWeight()));
    entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}, {var_id.first, var_id.second.at(1)}});
    return entries_.size() - 1;
  }

  size_t AddProfile(const Axis& x, const Axis& y, Cuts* cuts = nullptr, Variable weight = Variable{}) {
    return AddProfile("", x, y, cuts, weight);
  }

  size_t AddIntegral(const std::string& name, const Axis& x, Cuts* cuts = nullptr, Variable weight = Variable{}) {
    CreateOutputFileIfNotYet();
    weight.IfEmptyVariableConvertToOnes(x);
    entries_.emplace_back(EntryConfig(x, weight, name, cuts, true));
    TDirectory* dir = MkMultiLevelDir(out_file_, toplevel_dir_name_ + "/" + entries_.back().GetDirectoryName());
    ANALYSISTREE_UTILS_VISIT(setdirectory_struct(dir), entries_.back().GetPlot());
    ANALYSISTREE_UTILS_VISIT(setname_struct(entries_.back().GetName()), entries_.back().GetPlot());
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts(), entries_.back().GetVariableForWeight()));
    entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}});
    return entries_.size() - 1;
  }

  size_t AddIntegral(const Axis& x, Cuts* cuts = nullptr, Variable weight = Variable{}) {
    return AddIntegral("", x, cuts, weight);
  }

  size_t AddIntegral(const Axis& x, const Axis& y, Cuts* cuts_x = nullptr, Cuts* cuts_y = nullptr) {
    CreateOutputFileIfNotYet();
    entries_.emplace_back(EntryConfig(x, cuts_x, y, cuts_y));
    TDirectory* dir = MkMultiLevelDir(out_file_, toplevel_dir_name_ + "/" + entries_.back().GetDirectoryName());
    ANALYSISTREE_UTILS_VISIT(setdirectory_struct(dir), entries_.back().GetPlot());
    ANALYSISTREE_UTILS_VISIT(setname_struct(entries_.back().GetName()), entries_.back().GetPlot());
    auto var_id_x = AddEntry(AnalysisEntry({entries_.back().GetVariables()[0]}, cuts_x));
    auto var_id_y = AddEntry(AnalysisEntry({entries_.back().GetVariables()[1]}, cuts_y));
    entries_.back().SetVariablesId({{var_id_x.first, var_id_x.second.at(0)}, {var_id_y.first, var_id_y.second.at(0)}});
    return entries_.size() - 1;
  }

  std::vector<EntryConfig>& Entries() { return entries_; }
  void SetOutputFileName(std::string name) { out_file_name_ = std::move(name); }
  void SetTopLevelDirName(const std::string& name) { toplevel_dir_name_ = name; }
  void ResetTopLevelDirName() { toplevel_dir_name_ = ""; }

 private:
  void FillIntegral(EntryConfig& plot);
  TDirectory* MkMultiLevelDir(TFile* file, const std::string& name) const;

  template<typename T>
  TDirectory* MkDirIfNotExists(T* fod, std::string name) const {
    if(fod == nullptr) throw std::runtime_error("Task::MkDirIfNotExists(): file or directory ptr is null");
    TDirectory* result = fod->GetDirectory(name.c_str());
    if (result == nullptr) result = fod->mkdir(name.c_str());
    return result;
  }

  void CreateOutputFileIfNotYet();

  std::vector<EntryConfig> entries_{};
  std::map<std::string, TDirectory*> dir_map_{};
  std::string out_file_name_{"QA.root"};
  std::string toplevel_dir_name_{""};
  TFile* out_file_{nullptr};

  ClassDefOverride(Task, 1);
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SRC_TASK_HPP_
