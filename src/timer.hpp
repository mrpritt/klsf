/**
 * \file timer.hpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-11-05 10:46:21 ritt>
 */
#pragma once

#include <chrono>

typedef std::chrono::system_clock Clock;

struct timer {
  using clock = std::chrono::steady_clock;
  using time_point = clock::time_point;

  time_point tp_start;
  double tm_lim;

  timer(double timeLimitSecs, const timer &parent) { reset(std::min(timeLimitSecs, parent.left())); }
  timer(double time_lim_secs = std::numeric_limits<double>::max()) { reset(time_lim_secs); }

  void reset(double time_lim_secs = std::numeric_limits<double>::max()) { tm_lim = time_lim_secs, tp_start = clock::now(); }
  double elapsed() const { return std::chrono::duration_cast<std::chrono::duration<double>>(clock::now() - tp_start).count(); }
  double left() const { return tm_lim - elapsed(); }
};
