// Worked example for a private "custom" cut (see tasks/README.md's "Custom
// cuts" section). declarative {type: range}/{type: equals}/{type: not_equals}
// already cover a single field vs. a threshold/value - a custom cut is only
// needed for something they can't express, such as a RATIO of two fields
// (chi2/ndf below a threshold, here). vertex-chi2/hit-count/eta range checks
// below could equally be written as plain {type: range} entries; they're
// included here only to show several conditions combined into one cut.
//
// Structurally mirrors the GoodCentralityTracks cut hand-built in
// examples/example.cpp. This is deliberately an *example*, not a built-in
// (services/steer/tasks/) cut: which quantities make a "good track" is an
// analysis-specific choice, not something the framework should ship as a
// one-size-fits-all default - copy this file and adjust it to your own
// selection instead of expecting it to already exist.
//
// Referenced from YAML as:
//   {type: custom, name: GoodTrackQuality, branch: <Branch>,
//    vtx_chi2: {field: <field>, min: <num>, max: <num>},
//    nhits: {field: <field>, min: <num>, max: <num>},
//    chi2_over_ndf: {chi2_field: <field>, ndf_field: <field>, max: <num>},
//    eta: {field: <field>, min: <num>, max: <num>}}

#include "CutFactory.hpp"

namespace AnalysisTree {
namespace QA {

ATQA_REGISTER_CUT(GoodTrackQuality, [](const YAML::Node& params) {
  const auto branch = params["branch"].as<std::string>();
  const auto variable = [&branch](const std::string& field) { return branch + "." + field; };

  const auto vtx_chi2_node = params["vtx_chi2"];
  const auto vtx_chi2_var = variable(vtx_chi2_node["field"].as<std::string>());
  const auto vtx_chi2_min = vtx_chi2_node["min"].as<double>();
  const auto vtx_chi2_max = vtx_chi2_node["max"].as<double>();

  const auto nhits_node = params["nhits"];
  const auto nhits_var = variable(nhits_node["field"].as<std::string>());
  const auto nhits_min = nhits_node["min"].as<double>();
  const auto nhits_max = nhits_node["max"].as<double>();

  const auto chi2_over_ndf_node = params["chi2_over_ndf"];
  const auto chi2_var = variable(chi2_over_ndf_node["chi2_field"].as<std::string>());
  const auto ndf_var = variable(chi2_over_ndf_node["ndf_field"].as<std::string>());
  const auto chi2_over_ndf_max = chi2_over_ndf_node["max"].as<double>();

  const auto eta_node = params["eta"];
  const auto eta_var = variable(eta_node["field"].as<std::string>());
  const auto eta_min = eta_node["min"].as<double>();
  const auto eta_max = eta_node["max"].as<double>();

  return AnalysisTree::SimpleCut(
      {vtx_chi2_var, nhits_var, chi2_var, ndf_var, eta_var},
      [=](std::vector<double> v) {
        const double vtx_chi2 = v[0];
        const double nhits = v[1];
        const double chi2_over_ndf = v[2] / v[3];
        const double eta = v[4];
        return vtx_chi2 >= vtx_chi2_min && vtx_chi2 <= vtx_chi2_max
            && nhits >= nhits_min && nhits <= nhits_max
            && chi2_over_ndf < chi2_over_ndf_max
            && eta >= eta_min && eta <= eta_max;
      },
      "GoodTrackQuality");
});

}// namespace QA
}// namespace AnalysisTree
