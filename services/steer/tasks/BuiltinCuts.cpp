// Built-in, generic custom cut (see CutFactory.hpp) demonstrating why/how ATQA_REGISTER_CUT exists as a services/ capability, not just something available to your own task code: the declarative {type: range}/{equals}/
// {not_equals} checks (see services/README.md) only ever compare ONE field against a fixed value; they can't express a condition that combines two fields, such as a ratio. Unlike an analysis-specific combo cut (see
// services/README.md's "Custom cuts" section for that kind of example), "ratio of two fields in a range" is a generic, broadly reusable building block, so it ships as a built-in.

#include "CutFactory.hpp"

namespace AnalysisTree {
namespace QA {

// Referenced from YAML as:
//   {type: custom, name: RatioInRange, branch: <Branch>,
//    numerator_field: <field>, denominator_field: <field>, min: <num>, max: <num>}
ATQA_REGISTER_CUT(RatioInRange, [](const YAML::Node& params) {
  const auto branch = params["branch"].as<std::string>();
  const auto numerator = branch + "." + params["numerator_field"].as<std::string>();
  const auto denominator = branch + "." + params["denominator_field"].as<std::string>();
  const auto min = params["min"].as<double>();
  const auto max = params["max"].as<double>();

  return AnalysisTree::SimpleCut(
      {numerator, denominator},
      [min, max](std::vector<double> v) {
        const double ratio = v[0] / v[1];
        return ratio >= min && ratio <= max;
      },
      "RatioInRange");
});

}// namespace QA
}// namespace AnalysisTree
