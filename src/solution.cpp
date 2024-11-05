/**
 * \file solution.cpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-11-04 11:47:43 ritt>
 */
#include "solution.hpp"

using namespace std;

ShortPartialSolution::ShortPartialSolution(const PartialSolution &p) : g(p.g) { r = p.short_repr().r; }

bool operator==(const ShortPartialSolution &a, const ShortPartialSolution &b) { return a.r == b.r; }

void determine_k(Graph &g) {
  unsigned j = 0, ncc = 1;

  while (ncc == 1) {
    PartialSolution s(g);
    g.nLabels = g.nColors >> j;
    s.complete_greedy(1);
    ncc = s.value();
    j++;
  };
}
