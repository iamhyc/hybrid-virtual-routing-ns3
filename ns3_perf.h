#ifndef __NS3_PERF_H__
#define __NS3_PERF_H__

#include <cmath>

#include "ns3/names.h"
#include "ns3/type-id.h"
#include "ns3/object.h"
#include "ns3/uinteger.h"

#include "ns3/nstime.h"
#include "ns3/simulator.h"

#include "ns3/trace-source-accessor.h"
#include "ns3/traced-value.h"
#include "ns3/double-probe.h"
#include "ns3/basic-data-calculators.h"

namespace ns3_perf
{
	const double TOLERANCE = 1e-14;
	ns3::MinMaxAvgTotalCalculator<int> Icalculator;
	ns3::MinMaxAvgTotalCalculator<double> Fcalculator;
}

#endif