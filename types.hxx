#ifndef NETSIM_TYPES_HXX
#define NETSIM_TYPES_HXX
#include <functional>

using ElementID = int;
using Time = int;
using TimeOffset = int;
using PropabilityGenerator = std::function<double()>;

#endif 