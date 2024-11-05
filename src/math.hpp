/**
 * \file math.hpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br> 
 *   \date Time-stamp: <2024-06-10 08:37:04 ritt>
 */
#pragma once 

double log_binomial(int n, int k) {
  double r = 0;
  for (auto i = 0; i != n; i++)
    r += log(i+1);
  for (auto i = 0; i != k; i++)
    r -= log(i+1);
  for (auto i = 0; i != n-k; i++)
    r -= log(i+1);
  return r;
}

