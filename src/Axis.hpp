#ifndef ANALYSISTREEQA_AXIS_HPP
#define ANALYSISTREEQA_AXIS_HPP

#include "TAxis.h"
#include "AnalysisTree/Variable.hpp"

namespace AnalysisTree {
namespace QA {

class Axis : public Variable, public TAxis {
 public:
  Axis() = default;
  Axis(const std::string& title, const Variable& var, const TAxis& a) : Variable(var), TAxis(a) {
    this->SetTitle(title.c_str());
    if(this->GetFields().size() == 1 && this->GetFields().at(0).GetName() == "ones"){
      this->lambda_ = [](const std::vector<double>& ){ return 1; };
      this->name_ = this->GetBranchName() + ".ones";
    }
  }
  const char* GetName() const override { return Variable::GetName().c_str(); }
  void Print() const override ;

 protected:
  ClassDefOverride(Axis, 1);
};
}
}
#endif//ANALYSISTREEQA_AXIS_HPP
