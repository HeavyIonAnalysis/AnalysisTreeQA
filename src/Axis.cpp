#include "Axis.hpp"

namespace AnalysisTree {
namespace QA {

using std::cout;
using std::endl;
using std::setw;

void Axis::Print(Option_t*) const {
  cout << setw(25) << this->name_ << " ";
  cout << setw(6) << this->GetNbins() << setw(6) << this->GetXmin() << setw(6) << this->GetXmax();
}

}
}