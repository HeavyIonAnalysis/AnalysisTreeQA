#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

#include "EntryConfig.hpp"

namespace AnalysisTree {
namespace QA {

using std::cout;
using std::endl;
using std::setw;
using std::left;

struct fill_struct : public Utils::Visitor<void> {
  fill_struct(double val1, double val2) : val1_(val1), val2_(val2) {}
  void operator()(TH1*) const { throw std::runtime_error("Cannot apply Fill(va1, val2) to TH1"); }
  void operator()(TH2* h2) const { h2->Fill(val1_, val2_); }
  void operator()(TProfile* p) const { p->Fill(val1_, val2_); }
  double val1_, val2_;
};

struct write_struct : public Utils::Visitor<void> {
  explicit write_struct(std::string n) : name_(std::move(n)) {}
  template<class PlotType>
  void operator()(PlotType* p) const { p->Write(name_.c_str()); }
  std::string name_;
};

EntryConfig::EntryConfig(const Axis& axis, Cuts* cuts, bool is_integral, const std::string& name)
  : name_(name.empty() ? axis.GetName() : name),
    type_(is_integral ? PlotType::kIntegral1D : PlotType::kHisto1D),
    axes_({axis}),
    entry_cuts_(cuts) {
  if (cuts && name.empty())
    name_ += "_" + cuts->GetName();
  if(is_integral && name.empty()){
    name_ += "_integral";
  }
  InitPlot();
  Print();
}

EntryConfig::EntryConfig(const Axis& x, const Axis& y, Cuts* cuts, bool is_profile, const std::string& name)
  : type_(is_profile ? PlotType::kProfile : PlotType::kHisto2D),
    axes_({x, y}),
    entry_cuts_(cuts)
{
  name_ = name.empty() ? Construct2DName() : name;
  InitPlot();
  Print();
}

EntryConfig::EntryConfig(const Axis& x, Cuts* cuts_x, const Axis& y, Cuts* cuts_y, const std::string& name)
    : type_(PlotType::kIntegral2D),
      axes_({x, y}),
      entry_cuts_(cuts_x)
{
  name_ = name.empty() ? Construct2DName() : name;
  InitPlot();
  Print();
}

TH1* EntryConfig::CreateHisto1D() const {
  auto x = axes_.at(0);
  TH1* ret = x.IsVariableBinSize() ?
     new TH1F(name_.c_str(), title_.c_str(), x.GetNbins(), x.GetXbins()->GetArray()) :
     new TH1F(name_.c_str(), title_.c_str(), x.GetNbins(), x.GetXmin(), x.GetXmax());
  ret->SetXTitle(x.GetTitle());
  ret->SetYTitle("Entries");
  return ret;
}

TProfile* EntryConfig::CreateProfile() const {
  auto x{axes_.at(0)}, y{axes_.at(1)};
  auto ret = x.IsVariableBinSize() ?
        new TProfile(name_.c_str(), title_.c_str(), x.GetNbins(), x.GetXbins()->GetArray()) :
        new TProfile(name_.c_str(), title_.c_str(), x.GetNbins(), x.GetXmin(), x.GetXmax());

  ret->SetXTitle(x.GetTitle());
  ret->SetYTitle(y.GetTitle());
  return ret;
}

TH2* EntryConfig::CreateHisto2D() const {
  auto x{axes_.at(0)}, y{axes_.at(1)};
  assert(x.IsVariableBinSize() && y.IsVariableBinSize() || !x.IsVariableBinSize() && !y.IsVariableBinSize());

  auto* ret = x.IsVariableBinSize() ?
      new TH2F(name_.c_str(), title_.c_str(), x.GetNbins(), x.GetXbins()->GetArray(), y.GetNbins(), y.GetXbins()->GetArray()) :
      new TH2F(name_.c_str(), title_.c_str(), x.GetNbins(), x.GetXmin(), x.GetXmax(), y.GetNbins(), y.GetXmin(), y.GetXmax());

  ret->SetXTitle(x.GetTitle());
  ret->SetYTitle(y.GetTitle());
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

std::string EntryConfig::Construct2DName() {
  std::string name = Form("%s_%s", axes_[0].GetName(), axes_[1].GetName());
  if (entry_cuts_ != nullptr)
    name += "_" + entry_cuts_->GetName();

  if (type_ == PlotType::kProfile) {
    name += "_profile";
  }
  return name;
}

void EntryConfig::Fill(double value1, double value2) {
  ANALYSISTREE_UTILS_VISIT(fill_struct(value1, value2), plot_);
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

void EntryConfig::Print() const {
  cout << left;
  switch (type_) {
    case PlotType::kHisto1D : cout << setw(12) << "1D histo"; break;
    case PlotType::kHisto2D : cout << setw(12) << "2D histo"; break;
    case PlotType::kProfile : cout << setw(12) << "Profile"; break;
    case PlotType::kIntegral1D : cout << setw(12) << "Integral"; break;
    case PlotType::kIntegral2D : cout << setw(12) << "2D integral"; break;
  }
  for(const auto& axis : axes_){
    axis.Print();
  }
  if(entry_cuts_){
    cout << setw(15) << entry_cuts_->GetName();
  }
  cout << endl;
}

}// namespace QA
}// namespace AnalysisTree