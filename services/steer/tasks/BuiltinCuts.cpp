// Built-in example of a "custom" cut (see CutFactory.hpp): a predicate that
// cannot be expressed as a single declarative range/equals check. Mirrors
// the GoodCentralityTracks cut hand-built in examples/example.cpp.
//
// Referenced from YAML as: {type: custom, name: GoodTrackQuality}

#include "CutFactory.hpp"

namespace AnalysisTree {
namespace QA {

ATQA_REGISTER_CUT(GoodTrackQuality, [](const YAML::Node&) {
  return AnalysisTree::SimpleCut(
      {"VtxTracks.vtx_chi2", "VtxTracks.nhits", "VtxTracks.chi2", "VtxTracks.ndf", "VtxTracks.eta"},
      [](std::vector<double> v) {
        const double vtx_chi2 = v[0];
        const double nhits = v[1];
        const double chi2_over_ndf = v[2] / v[3];
        const double eta = v[4];
        return vtx_chi2 >= 0 && vtx_chi2 <= 3 && nhits >= 4 && nhits <= 100 && chi2_over_ndf < 10 && eta >= 0 && eta <= 3;
      },
      "GoodTrackQuality");
});

}// namespace QA
}// namespace AnalysisTree
