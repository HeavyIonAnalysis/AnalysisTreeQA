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

  void Exec() override;
  void Finish() override;

  size_t AddH1(const std::string& name, const Axis& x, Cuts* cuts = nullptr, Variable weight = Variable{});

  size_t AddH1(const Axis& x, Cuts* cuts = nullptr, Variable weight = Variable{});

  size_t AddH2(const std::string& name, const Axis& x, const Axis& y, Cuts* cuts = nullptr, Variable weight = Variable{});

  size_t AddH2(const Axis& x, const Axis& y, Cuts* cuts = nullptr, Variable weight = Variable{});

  size_t AddProfile(const std::string& name, const Axis& x, const Axis& y, Cuts* cuts = nullptr, Variable weight = Variable{});

  size_t AddProfile(const Axis& x, const Axis& y, Cuts* cuts = nullptr, Variable weight = Variable{});

  size_t AddIntegral(const std::string& name, const Axis& x, Cuts* cuts = nullptr, Variable weight = Variable{});

  size_t AddIntegral(const Axis& x, Cuts* cuts = nullptr, Variable weight = Variable{});

  size_t AddIntegral(const Axis& x, const Axis& y, Cuts* cuts_x = nullptr, Cuts* cuts_y = nullptr);

  std::vector<EntryConfig>& Entries() { return entries_; }
  void SetOutputFileName(std::string name, std::string option = "recreate") {
    out_file_name_ = std::move(name);
    out_file_option_ = std::move(option);
  }
  void SetTopLevelDirName(const std::string& name, bool is_append_dir_name_with_entry_name = false) {
    toplevel_dir_name_ = name;
    is_append_dir_name_with_entry_name_ = is_append_dir_name_with_entry_name;
  }
  void ResetTopLevelDirName() {
    toplevel_dir_name_ = "";
    is_append_dir_name_with_entry_name_ = false;
  }

 private:
  void FillIntegral(EntryConfig& plot);
  TDirectory* MkMultiLevelDir(TFile* file, const std::string& name) const;

  template<typename T>
  TDirectory* MkDirIfNotExists(T* fod, std::string name) const {
    if (fod == nullptr) throw std::runtime_error("Task::MkDirIfNotExists(): file or directory ptr is null");
    TDirectory* result = fod->GetDirectory(name.c_str());
    if (result == nullptr) result = fod->mkdir(name.c_str());
    return result;
  }

  void CreateOutputFileIfNotYet();
  std::string ConstructOutputDirectoryName();

  std::vector<EntryConfig> entries_{};
  std::map<std::string, TDirectory*> dir_map_{};
  std::string out_file_name_{"QA.root"};
  std::string out_file_option_{"recreate"};
  std::string toplevel_dir_name_{""};
  bool is_append_dir_name_with_entry_name_{false};
  TFile* out_file_{nullptr};

  ClassDefOverride(Task, 1);
};

}// namespace QA
}// namespace AnalysisTree

#endif//ANALYSISTREEQA_SRC_TASK_HPP_
