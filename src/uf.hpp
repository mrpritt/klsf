/**
 * \file uf.hpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-11-04 11:46:16 ritt>
 */
#pragma once

#include <algorithm>
#include <vector>

struct uf {
  std::vector<int> p; 
  unsigned nc;        

  uf(int n) : p(n, -1), nc(n) {}

  int find(int _e) {
    if (p[_e] < 0)
      return _e;
    int e = _e;
    while (p[e] >= 0)
      e = p[e];
    p[_e] = e;
    return e;
  }

  void unite(int e, int f) {
    e = find(e);
    f = find(f);

    if (e != f) {
      int re = -p[e], rf = -p[f];
      if (re < rf)
        std::swap(e, f);

      p[f] = e;
      
      if (re == rf)
        p[e]--;
      nc--;
    }
  }

  unsigned nComp() const { return nc; }
  unsigned size() const { return p.size(); }
};
