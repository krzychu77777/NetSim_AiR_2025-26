#ifndef NETSIM_TYPES_HXX
#define NETSIM_TYPES_HXX
#include <functional>

using ElementID = int;
using Time = int;
using TimeOffset = int;
using ProbabilityGenerator = std::function<double()>;

#endif 