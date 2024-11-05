/**
 * \file utilities.hpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br> 
 *   \date Time-stamp: <2024-11-04 11:55:04 ritt>
 */
#pragma once

#include <sstream>
#include <limits>

template <class charT, class traits>
inline std::basic_istream<charT, traits> &ignoreToken (std::basic_istream<charT, traits> &strm) {
  strm.ignore (std::numeric_limits<long int>::max(), ' ');
  return strm;
}
