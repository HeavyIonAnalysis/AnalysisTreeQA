#ifndef ANALYSISTREEQA_SRC_AXIS_HPP_
#define ANALYSISTREEQA_SRC_AXIS_HPP_

#include "AnalysisTree/Variable.hpp"
#include "TAxis.h"

namespace AnalysisTree {
namespace QA {

class Axis : public Variable, public TAxis {

 public:
  Axis() = default;

  Axis(const std::string& title, const Variable& var, const TAxis& a) : Variable(var), TAxis(a) {
    this->SetTitle(title.c_str());
  }

  const char* GetName() const override { return Variable::GetName().c_str(); }
};

}// namespace QA
}// namespace AnalysisTree
#endif//ANALYSISTREEQA_SRC_AXIS_HPP_
