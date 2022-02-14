#ifndef ANALYSISTREEQA_SRC_TASK_HPP_
#define ANALYSISTREEQA_SRC_TASK_HPP_

#include <utility>

#include "TFile.h"

#include "AnalysisTree/AnalysisTask.hpp"

#include "EntryConfig.hpp"

namespace AnalysisTree {
namespace QA {

class Task : public AnalysisTask {
 public:
  Task() = default;

  void Init() override;
  void Exec() override;
  void Finish() override;

  size_t AddH1(const Axis& x, Cuts* cuts = nullptr, const std::string& name = "") {
    entries_.emplace_back(EntryConfig(x, cuts, false, name));
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts()));
    entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}});
    return entries_.size() - 1;
  }

  size_t AddH2(const Axis& x, const Axis& y, Cuts* cuts = nullptr, const std::string& name = "") {
    entries_.emplace_back(EntryConfig(x, y, cuts, false, name));
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts()));
    entries_.back().SetVariablesId({ {var_id.first, var_id.second.at(0)}, {var_id.first, var_id.second.at(1)} });
    return entries_.size() - 1;
  }

  size_t AddProfile(const Axis& x, const Axis& y, Cuts* cuts = nullptr, const std::string& name = "") {
    entries_.emplace_back(EntryConfig(x, y, cuts, true, name));
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts()));
    entries_.back().SetVariablesId({ {var_id.first, var_id.second.at(0)}, {var_id.first, var_id.second.at(1)} });
    return entries_.size() - 1;
  }

  size_t AddIntegral(const Axis& x, Cuts* cuts = nullptr, const std::string& name = "") {
    entries_.emplace_back(EntryConfig(x, cuts, true, name));
    auto var_id = AddEntry(AnalysisEntry(entries_.back().GetVariables(), entries_.back().GetEntryCuts()));
    entries_.back().SetVariablesId({{var_id.first, var_id.second.at(0)}});
    return entries_.size() - 1;
  }

  size_t AddIntegral(const Axis& x, const Axis& y,
                     Cuts* cuts_x = nullptr, Cuts* cuts_y = nullptr,
                     const std::string& name = "") {
    entries_.emplace_back(EntryConfig(x, cuts_x, y, cuts_y, name));
    auto var_id_x = AddEntry(AnalysisEntry({entries_.back().GetVariables()[0]}, cuts_x));
    auto var_id_y = AddEntry(AnalysisEntry({entries_.back().GetVariables()[1]}, cuts_y));
    entries_.back().SetVariablesId({ {var_id_x.first, var_id_x.second.at(0)}, {var_id_y.first, var_id_y.second.at(0)} });
    return entries_.size() - 1;
  }

  std::vector<EntryConfig>& Entries() { return entries_; }
  void SetOutputFileName(std::string name) { out_file_name_ = std::move(name); }

  void AddCut(Cuts* cut){
    auto lb = cuts_.lower_bound(cut->GetName());
    if(lb != cuts_.end() && !(cuts_.key_comp()(cut->GetName(), lb->first))) {
      throw std::runtime_error("Cut already exists: " + cut->GetName());
    }
    else {
      cuts_.insert(lb, {cut->GetName(), cut});
    }
  }

  void AddVariable(const Variable& variable){
    auto lb = vars_.lower_bound(variable.GetName());
    if(lb != vars_.end() && !(vars_.key_comp()(variable.GetName(), lb->first))) {
      throw std::runtime_error("Variable already exists: " + variable.GetName());
    }
    else {
      vars_.insert(lb, {variable.GetName(), variable});
    }
  }

  [[nodiscard]] Cuts* GetCut(const std::string& name) const {
    auto it = cuts_.find(name);
    if(it == cuts_.end()){
      throw std::runtime_error("Cut is not found: " + name);
    }
    return it->second;
  }

  [[nodiscard]] Variable GetVariable(const std::string& name) const {
    auto it = vars_.find(name);
    if(it == vars_.end()){
      return Variable::FromString(name);
    }
    return it->second;
  }

 private:
  void FillIntegral(EntryConfig& plot);
  size_t AddAnalysisEntry(const Axis& a, Cuts* cuts, bool is_integral);

  std::vector<EntryConfig> entries_{};
  std::map<std::string, TDirectory*> dir_map_{};
  std::string out_file_name_{"QA.root"};
  TFile* out_file_{nullptr};

  std::map<std::string, Cuts*> cuts_{};
  std::map<std::string, Variable> vars_{};

  ClassDefOverride(Task, 1);
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SRC_TASK_HPP_
