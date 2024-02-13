#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

#include "EntryConfig.hpp"

namespace AnalysisTree {
namespace QA {

struct fill2_struct : public Utils::Visitor<void> {
  fill2_struct(double val1, double val2) : val1_(val1), val2_(val2) {}
  void operator()(TH1* h1) const { h1->Fill(val1_, val2_); }
  void operator()(TH2* h2) const { h2->Fill(val1_, val2_); }
  void operator()(TProfile* p) const { p->Fill(val1_, val2_); }
  double val1_, val2_;
};

struct fill3_struct : public Utils::Visitor<void> {
  fill3_struct(double val1, double val2, double val3) : val1_(val1), val2_(val2), val3_(val3) {}
  void operator()(TH1*) const { throw std::runtime_error("EntryConfig, fill3_struct: cannot fill TH1 with 3 arguments"); }
  void operator()(TH2* h2) const { h2->Fill(val1_, val2_, val3_); }
  void operator()(TProfile* p) const { p->Fill(val1_, val2_, val3_); }
  double val1_, val2_, val3_;
};

struct write_struct : public Utils::Visitor<void> {
  explicit write_struct(std::string n) : name_(std::move(n)) {}
  template<class PlotType>
  void operator()(PlotType* p) const { p->Write(name_.c_str()); }
  std::string name_;
};

EntryConfig::EntryConfig(const Axis& axis, Variable& weight, Cuts* cuts, bool is_integral)
  : name_(axis.GetName()),
    type_(is_integral ? PlotType::kIntegral1D : PlotType::kHisto1D),
    axes_({axis}),
    var4weight_(weight),
    entry_cuts_(cuts) {
  if (cuts)
    name_ += "_" + cuts->GetName();
  if(is_integral){
    name_ += "_integral";
  }
  InitPlot();
}

EntryConfig::EntryConfig(const Axis& x, const Axis& y, Variable& weight, Cuts* cuts, bool is_profile) : type_(is_profile ? PlotType::kProfile : PlotType::kHisto2D),
                                                                                      axes_({x, y}),
                                                                                      var4weight_(weight),
                                                                                      entry_cuts_(cuts) {
  Set2DName();
  InitPlot();
}

EntryConfig::EntryConfig(const Axis& x, Cuts* cuts_x, const Axis& y, Cuts* cuts_y) : type_(PlotType::kIntegral2D),
                                                                                      axes_({x, y}),
                                                                                      entry_cuts_(cuts_x) {
  Set2DName();
  InitPlot();
}



TH1* EntryConfig::CreateHisto1D() const {
  auto* ret = new TH1F(name_.c_str(), title_.c_str(),
                       axes_.at(0).GetNbins(), axes_.at(0).GetXmin(), axes_.at(0).GetXmax());
  ret->SetXTitle(axes_.at(0).GetTitle());
  ret->SetYTitle("Entries");
  return ret;
}
// TODO fix axes
TProfile* EntryConfig::CreateProfile() const {

  TProfile* ret{nullptr};
  if (axes_[1].GetNbins() == 1 && axes_[1].GetXmax() == 1. && axes_[1].GetXmin() == 0.) {// Not init by user
    ret = new TProfile(name_.c_str(), title_.c_str(),
                       axes_.at(0).GetNbins(), axes_.at(0).GetXmin(), axes_.at(0).GetXmax());
  } else if (axes_.size() == 2) {
    ret = new TProfile(name_.c_str(), title_.c_str(),
                       axes_.at(0).GetNbins(), axes_.at(0).GetXmin(), axes_.at(0).GetXmax(),
                       axes_.at(1).GetXmin(), axes_.at(1).GetXmax());
  }
  ret->SetYTitle(axes_.at(1).GetTitle());
  ret->SetXTitle(axes_.at(0).GetTitle());
  return ret;
}

TH2* EntryConfig::CreateHisto2D() const {

  auto* ret = new TH2F(name_.c_str(), title_.c_str(),
                       axes_.at(0).GetNbins(), axes_.at(0).GetXmin(), axes_.at(0).GetXmax(),
                       axes_.at(1).GetNbins(), axes_.at(1).GetXmin(), axes_.at(1).GetXmax());
  ret->SetXTitle(axes_.at(0).GetTitle());
  ret->SetYTitle(axes_.at(1).GetTitle());
  ret->SetZTitle("Entries");
  ret->SetMinimum(1);
  return ret;
}

void EntryConfig::InitPlot() {
  switch (type_) {
    case PlotType::kHisto1D: {
      plot_ = CreateHisto1D();
      break;
    }
    case PlotType::kHisto2D: {
      plot_ = CreateHisto2D();
      break;
    }
    case PlotType::kIntegral1D: {
      plot_ = CreateHisto1D();
      break;
    }
    case PlotType::kIntegral2D: {
      plot_ = CreateHisto2D();
      break;
    }
    case PlotType::kProfile: {
      plot_ = CreateProfile();
      break;
    }
    default: {
      throw std::runtime_error("Wrong plot type!");
    }
  }
}

void EntryConfig::Set2DName() {
  name_ = Form("%s_%s", axes_[0].GetName(), axes_[1].GetName());
  if (entry_cuts_ != nullptr)
    name_ += "_" + entry_cuts_->GetName();

  if (type_ == PlotType::kProfile) {
    name_ += "_profile";
  }
}

void EntryConfig::Fill(double value1, double value2) {
  ANALYSISTREE_UTILS_VISIT(fill2_struct(value1, value2), plot_);
}

void EntryConfig::Fill(double value1, double value2, double value3) {
  ANALYSISTREE_UTILS_VISIT(fill3_struct(value1, value2, value3), plot_);
}

void EntryConfig::Write() const {
  assert(out_dir_);
  out_dir_->cd();
  ANALYSISTREE_UTILS_VISIT(write_struct(name_), plot_);
}

void EntryConfig::Fill(double value) { ANALYSISTREE_UTILS_GET<TH1*>(plot_)->Fill(value); }

std::string EntryConfig::GetDirectoryName() const {

  std::set<std::string> branches{};
  for (const auto& ax : axes_) {
    const auto& br = ax.GetBranches();
    branches.insert(br.begin(), br.end());
  }
  std::string name{*branches.begin()};
  for (auto it = ++branches.begin(); it != branches.end(); ++it) {
    name += "_" + *it;
  }
  if (entry_cuts_) {
    name += "_" + entry_cuts_->GetName();
  }
  return name;
}

}// namespace QA
}// namespace AnalysisTree