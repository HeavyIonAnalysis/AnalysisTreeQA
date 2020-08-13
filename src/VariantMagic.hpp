#ifndef ANALYSISTREEQA_SRC_VARIANTMAGIC_HPP_
#define ANALYSISTREEQA_SRC_VARIANTMAGIC_HPP_

// I'm really sorry about this, blame Oleg
#ifdef USEBOOST
#include "AnalysisTree/Detector.hpp"
#include "AnalysisTree/EventHeader.hpp"
#include "boost/variant.hpp"

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"

#pragma message("Compiling with Boost")
#define get_func boost::get
using namespace AnalysisTree;

struct fill_struct : public boost::static_visitor<> {
  fill_struct(double val1, double val2) : val1_(val1), val2_(val2) {}
  void operator()(TH1*) const {}
  void operator()(TH2* h2) const { h2->Fill(val1_, val2_); }
  void operator()(TProfile* p) const { p->Fill(val1_, val2_); }
  double val1_, val2_;
};
struct write_struct : public boost::static_visitor<> {
  explicit write_struct(std::string n) : name_(std::move(n)) {}
  void operator()(TH1* h1) const { h1->Write(name_.c_str()); }
  void operator()(TH2* h2) const { h2->Write(name_.c_str()); }
  void operator()(TProfile* p) const { p->Write(name_.c_str()); }
  std::string name_;
};
#else
#include <variant>
#pragma message("Compiling with c++17")
#define get_func std::get
#endif

#endif//ANALYSISTREEQA_SRC_VARIANTMAGIC_HPP_
