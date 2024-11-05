/**
 * \file logging.hpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-11-05 11:01:36 ritt>
 */
#pragma once

#include "timer.hpp"

#define FMT_HEADER_ONLY
#include "fmt/format.h"
#include "fmt/ostream.h"

namespace logging {
extern timer start;
inline double elapsed() { return start.elapsed(); }
}
