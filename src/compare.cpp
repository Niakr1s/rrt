#include "compare.h"

#include <math.h>
#include <algorithm>

namespace rrt {

bool compare(double x, double y) {
  double maxXYOne = std::max({1.0, std::fabs(x), std::fabs(y)});
  return std::fabs(x - y) <= std::numeric_limits<double>::epsilon() * maxXYOne;
}

}  // namespace rrt
