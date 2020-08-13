#ifndef ANALYSISTREEQA_SRC_TASK_HPP_
#define ANALYSISTREEQA_SRC_TASK_HPP_

#include "TFile.h"

#include "AnalysisTree/FillTask.hpp"
#include "AnalysisTree/VarManager.hpp"

#include "EntryConfig.hpp"

namespace AnalysisTree {
namespace QA {

class Task : public FillTask {
 public:
  Task() = default;

  void Init(std::map<std::string, void*>&) override;
  void Exec() override;
  void Finish() override;

  size_t AddEntry(const EntryConfig& entry) {
    entries_.push_back(entry);
    return entries_.size() - 1;
  }

  size_t AddH1(const Axis& x, Cuts* cuts = nullptr) {
    entries_.emplace_back(EntryConfig(x, cuts));
    return entries_.size() - 1;
  }

  size_t AddH2(const Axis& x, const Axis& y, Cuts* cuts = nullptr) {
    entries_.emplace_back(EntryConfig(x, y, cuts));
    return entries_.size() - 1;
  }

  size_t AddProfile(const Axis& x, const Axis& y, Cuts* cuts = nullptr) {
    entries_.emplace_back(EntryConfig(x, y, cuts, true));
    return entries_.size() - 1;
  }

  std::vector<EntryConfig>& Entries() { return entries_; }
  void SetPointerToVarManager(VarManager* ptr) { var_manager_ = ptr; }

 private:
  std::vector<EntryConfig> entries_{};
  VarManager* var_manager_{nullptr};
  std::map<std::string, TDirectory*> dir_map_{};
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SRC_TASK_HPP_
