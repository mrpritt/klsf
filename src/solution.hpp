/**
 * \file solution.hpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-11-05 10:41:49 ritt>
 */
#pragma once

#include <boost/dynamic_bitset.hpp>
using namespace boost;

#include "graph.hpp"
#include "logging.hpp"
#include "random.hpp"
#include "utilities.hpp"
using namespace logging;

struct PartialSolution;

struct ShortPartialSolution {
  const Graph &g;
  dynamic_bitset<> r;

  ShortPartialSolution(const Graph &g) : g(g), r(g.nColors) {}
  ShortPartialSolution(const PartialSolution &p);
};

bool operator==(const ShortPartialSolution &a, const ShortPartialSolution &b);

struct PartialSolution {
  const Graph &g;
  std::vector<unsigned> col;
  unsigned ncol;
  unsigned nedge;
  uf U;

  PartialSolution(const Graph &g) : g(g), col(g.nColors), ncol(0), nedge(0), U(g.nNodes) { iota(col.begin(), col.end(), 0); }

  PartialSolution(const ShortPartialSolution &s) : g(s.g), col(s.g.nColors), ncol(0), nedge(0), U(s.g.nNodes) {
    iota(col.begin(), col.end(), 0);
    for (auto i = 0ul, ie = col.size(); i != ie; ++i)
      if (s.r[i]) {
        auto p = find(col.begin(), col.end(), i);
        commitColorAt(p - col.begin());
      }
  }

  PartialSolution(PartialSolution &&p) : g(p.g), U(p.g.nNodes) { this->swap(p); }

  PartialSolution(const PartialSolution &p) : g(p.g), U(p.g.nNodes) {
    col = p.col;
    ncol = p.ncol;
    nedge = p.nedge;
    U = p.U;
  }

  PartialSolution &operator=(PartialSolution p) {
    this->swap(p);
    return *this;
  }

  void swap(PartialSolution &p) {
    using std::swap;
    col.swap(p.col);
    swap(ncol, p.ncol);
    swap(nedge, p.nedge);
    swap(U, p.U);
  }

  unsigned value() const { return U.nComp(); }

  bool isUsed(unsigned c) const {
    for (auto i = 0u; i != ncol; i++)
      if (col[i] == c)
        return true;
    return false;
  }

  std::string to_string() const {
    std::string r;
    for (auto i = 0u; i != g.nColors; i++)
      r += isUsed(i) ? "⬜" : "⬛";
    return r;
  }

  ShortPartialSolution short_repr() const {
    ShortPartialSolution s(g);
    for (auto i = 0u; i != ncol; i++)
      s.r.set(col[i]);
    return s;
  }

  void applyColorAt(unsigned i) { ::applyColor(g, col[i], U); }
  void applyColor(unsigned c) { ::applyColor(g, c, U); }

  void unselectColorAt(unsigned i) {
    if (i != ncol - 1)
      std::swap(col[i], col[ncol - 1]);
    ncol--;
  }

  void removeColorAt(unsigned i) {
    unselectColorAt(i);

    U = uf(g.nNodes);
    for (auto i = 0u; i != ncol; ++i)
      applyColorAt(i);
  }

  void commitColorAt(unsigned i) {
    applyColorAt(i);
    std::swap(col[i], col[ncol]);
    ncol++;
    nedge += g.ce[col[i]].size();
  }

  bool step(bool best_improvement = false) {
    PartialSolution best(*this);
    bool found_best = false;
    for (unsigned i = 0u; i != ncol; i++) {
      PartialSolution s(*this);
      s.removeColorAt(i);
      for (unsigned j = ncol + 1, je = col.size(); j != je; j++) {
        PartialSolution t(s);
        t.commitColorAt(j);
        if (t.value() < best.value()) {
          if (!best_improvement) {
            (*this) = t;
            return true;
          }
          best = t;
          found_best = true;
        }
      }
    }
    (*this) = best;
    return found_best;
  }

  using qelement = std::tuple<unsigned, int, unsigned>;

  unsigned selectColor(unsigned k) {
    std::vector<qelement> q;

    for (unsigned i = ncol, ie = col.size(); i != ie; i++) {
      uf V(U);
      ::applyColor(g, col[i], V);
      qelement nel{V.nComp(), -int(g.ce[col[i]].size()), i};

      if (q.size() < k)
        q.push_back(nel);
      else if (nel < q[0]) {
        pop_heap(q.begin(), q.end());
        q.back() = nel;
      }
      push_heap(q.begin(), q.end());
    }
    sort(q.begin(), q.end());
    return std::get<2>(q[::getRandom(0, q.size() - 1)]);
  }

  void complete_greedy(unsigned k) {
    while (ncol < g.nLabels) {
      int ci = selectColor(k);
      commitColorAt(ci);
    }
  }
};

namespace std {
template <> struct hash<ShortPartialSolution> {
  using argument_type = ShortPartialSolution;
  using result_type = size_t;

  result_type operator()(const argument_type &s) const { return hash<dynamic_bitset<>>()(s.r); }
};
}

void determine_k(Graph &g);
