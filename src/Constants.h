#ifndef ANALYSISTREEQA_SRC_CONSTANTS_H_
#define ANALYSISTREEQA_SRC_CONSTANTS_H_

constexpr int gNbins = 500;

// I'm really sorry about this, blame Oleg
#ifdef USEBOOST
  #include "boost/variant.hpp"
  #include "AnalysisTree/Detector.h"
  #include "AnalysisTree/EventHeader.h"
  #include "TChain.h"

  #pragma message ("Compiling with Boost")
  #define get_func boost::get
  using namespace AnalysisTree;

  template<typename T>
  struct get_field_struct : public boost::static_visitor<T> {
    get_field_struct(int i_channel, int field_id) : i_channel_(i_channel), field_id_(field_id) {}
    template <class Det>
    T get_field(Det* d) const { return d->GetChannel(i_channel_).template GetField<T>(field_id_); }
    T operator()(TrackDetector* d) const { return get_field<TrackDetector>(d); }
    T operator()(HitDetector* d) const { return get_field<HitDetector>(d); }
    T operator()(ModuleDetector* d) const { return get_field<ModuleDetector>(d); }
    T operator()(Particles* d) const { return get_field<Particles>(d); }
    T operator()(EventHeader* d) const { return get_field<EventHeader>(d); }
    int i_channel_, field_id_;
  };

  struct apply_cut : public boost::static_visitor<bool> {
    apply_cut(int i_channel, const Cuts* cut) : i_channel_(i_channel), cut_(cut) {}
    template <class Det>
    bool apply(Det* d) const { return cut_->Apply(d->GetChannel(i_channel_)); }
    bool operator()(TrackDetector* d) const { return apply<TrackDetector>(d); }
    bool operator()(HitDetector* d) const { return apply<HitDetector>(d); }
    bool operator()(ModuleDetector* d) const { return apply<ModuleDetector>(d); }
    bool operator()(Particles* d) const { return apply<Particles>(d); }
    bool operator()(EventHeader* d) const { return apply<EventHeader>(d); }
    int i_channel_;
    const Cuts* cut_;
  };

  struct get_n_channels_struct : public boost::static_visitor<size_t> {
    template <class Det>
    size_t get_n_channels(Det* d) const { return d->GetNumberOfChannels(); }
    size_t operator()(TrackDetector* d) const { return get_n_channels<TrackDetector>(d); }
    size_t operator()(HitDetector* d) const { return get_n_channels<HitDetector>(d); }
    size_t operator()(ModuleDetector* d) const { return get_n_channels<ModuleDetector>(d); }
    size_t operator()(Particles* d) const { return get_n_channels<Particles>(d); }
    size_t operator()(EventHeader* d) const { return get_n_channels<EventHeader>(d); }
  };
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
  #pragma message ("Compiling with c++17")
  #define get_func std::get
#endif

#endif //ANALYSISTREEQA_SRC_CONSTANTS_H_
