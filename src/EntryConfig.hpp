#ifndef ANALYSISTREE_QA_ENTRYCONFIG_H
#define ANALYSISTREE_QA_ENTRYCONFIG_H

#include <string>
#include <vector>

#include <TAxis.h>
#include <TH1.h>
#include <TH2.h>
#include <TProfile.h>

#include "AnalysisTree/Cuts.hpp"
#include "AnalysisTree/Utils.hpp"

namespace AnalysisTree::QA {

class Axis : public Variable, public TAxis {
 public:
  Axis() = default;
  Axis(const std::string& title, const Variable& var, const TAxis& a) : Variable(var), TAxis(a) {
    this->SetTitle(title.c_str());
    if (this->GetFields().size() == 1 && this->GetFields().at(0).GetName() == "ones") {
      this->lambda_ = [](const std::vector<double>&) { return 1; };
      this->name_ = "Ones";
    }
  }
  const char* GetName() const override { return Variable::GetName().c_str(); }

 protected:
  ClassDefOverride(Axis, 1);
};

class EntryConfig {

  using PlotPointer = ANALYSISTREE_UTILS_VARIANT<TH1*, TH2*, TProfile*>;

 public:
  enum class PlotType : short {
    kHisto1D,
    kHisto2D,
    kProfile,
    kIntegral1D,
    kIntegral2D
  };

  EntryConfig() = default;
  explicit EntryConfig(const Axis& axis, [[maybe_unused]] Variable& weight, const std::string& name, Cuts* cuts = nullptr, bool is_integral = false);
  EntryConfig(const Axis& x, const Axis& y, Variable& weight, const std::string& name, Cuts* cuts = nullptr, bool is_profile = false);
  EntryConfig(const Axis& x, Cuts* cuts_x, const Axis& y, Cuts* cuts_y);

  EntryConfig(const EntryConfig&) = default;
  EntryConfig(EntryConfig&&) = default;
  EntryConfig& operator=(EntryConfig&&) = default;
  EntryConfig& operator=(const EntryConfig&) = default;
  virtual ~EntryConfig() = default;

  void Fill(double value);
  void Fill(double value1, double value2);
  void Fill(double value1, double value2, double value3);

  ANALYSISTREE_ATTR_NODISCARD unsigned int GetNdimensions() const { return axes_.size(); }
  ANALYSISTREE_ATTR_NODISCARD Cuts* GetEntryCuts() const { return entry_cuts_; }
  ANALYSISTREE_ATTR_NODISCARD PlotType GetType() const { return type_; }

  ANALYSISTREE_ATTR_NODISCARD std::vector<std::pair<int, int>> GetVariablesId() const { return vars_id_; }
  void SetVariablesId(const std::vector<std::pair<int, int>>& var_id) { vars_id_ = var_id; }

  ANALYSISTREE_ATTR_NODISCARD std::vector<Variable> GetVariables() const {
    std::vector<Variable> vars{};
    for (const auto& axis : axes_) {
      vars.emplace_back(axis);
    }
    return vars;
  }

  ANALYSISTREE_ATTR_NODISCARD Variable GetVariableForWeight() const {
    return var4weight_;
  }

  ANALYSISTREE_ATTR_NODISCARD std::string GetDirectoryName() const;

  PlotPointer GetPlot() { return plot_; }

  const std::string& GetName() const { return name_; }

 protected:
  void InitPlot();
  ANALYSISTREE_ATTR_NODISCARD TH1* CreateHisto1D() const;
  ANALYSISTREE_ATTR_NODISCARD TH2* CreateHisto2D() const;
  ANALYSISTREE_ATTR_NODISCARD TProfile* CreateProfile() const;
  void Set2DName(const std::string& name = "");

  PlotPointer plot_;
  std::string name_;///< Name of the plot
  std::string title_;
  PlotType type_{PlotType(-1)};

  std::vector<Axis> axes_{};
  Variable var4weight_{};
  Cuts* entry_cuts_{nullptr};
  std::vector<std::pair<int, int>> vars_id_{};

  ClassDef(EntryConfig, 1);
};

}// namespace AnalysisTree::QA
#endif//ANALYSISTREE_QA_ENTRYCONFIG_H
