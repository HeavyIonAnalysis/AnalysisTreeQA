#ifndef ANALYSISTREE_QA_ENTRYCONFIG_H
#define ANALYSISTREE_QA_ENTRYCONFIG_H

#include <string>
#include <vector>

#include "AnalysisTree/Cuts.hpp"

#include "Axis.hpp"
#include "VariantMagic.hpp"

class TH1;
class TH2;
class TProfile;

namespace AnalysisTree {
namespace QA {

class EntryConfig {

#ifdef USEBOOST
  using PlotPointer = boost::variant<TH1*, TH2*, TProfile*>;
#else
  using PlotPointer = std::variant<TH1*, TH2*, TProfile*>;
#endif

 public:
  enum class PlotType : short {
    kHisto1D,
    kHisto2D,
    kProfile
  };

  EntryConfig() = default;

  explicit EntryConfig(const Axis& axis, Cuts* cuts = nullptr);
  EntryConfig(const Axis& x, const Axis& y, Cuts* cuts = nullptr, bool is_profile = false);

  void Fill(double value);
  void Fill(double value1, double value2);
  void Write() const;

  const std::vector<Axis>& GetAxes() const { return axes_; }
  std::vector<Axis>& Axes() { return axes_; }

  uint GetNdimentions() const { return axes_.size(); }
  Cuts* GetEntryCuts() const { return entry_cuts_; }
  PlotType GetType() const { return type_; }

  std::pair<int, std::vector<int>> GetVariablesId() const { return vars_id_; }
  void SetVariablesId(const std::pair<int, std::vector<int>>& var_id) { vars_id_ = var_id; }

  std::vector<Variable> GetVariables() const {
    std::vector<Variable> vars{};
    for (const auto& axis : axes_) {
      vars.emplace_back(axis);
    }
    return vars;
  }

  std::string GetDirectoryName() const;

  void SetOutDir(TDirectory* out_dir) { out_dir_ = out_dir; }

  PlotPointer GetPlot() { return plot_; }

 protected:
  void InitPlot();
  TH1* CreateHisto1D() const;
  TH2* CreateHisto2D() const;
  TProfile* CreateProfile() const;
  void Set2DName();

  PlotPointer plot_;
  std::string name_;///< Name of the plot
  std::string title_;
  PlotType type_{PlotType(-1)};

  std::vector<Axis> axes_{};
  Cuts* entry_cuts_{nullptr};
  std::pair<int, std::vector<int>> vars_id_{};

  TDirectory* out_dir_{nullptr};
};

}// namespace QA
}// namespace AnalysisTree
#endif//ANALYSISTREE_QA_ENTRYCONFIG_H
