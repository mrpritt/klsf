/**
 * \file cbfs.cpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-11-05 10:35:45 ritt>
 */
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <regex>
using namespace std;
namespace fs = std::filesystem;

#include <boost/heap/pairing_heap.hpp>
using namespace boost;

#include "cbfs-options.hpp"
#include "math.hpp"
#include "solution.hpp"
using namespace logging;

struct compare_ps {
  bool operator()(const PartialSolution &n1, const PartialSolution &n2) const { return n1.value() > n2.value() || (n1.value() == n2.value() && n1.nedge > n2.nedge); }
};

using Heap = boost::heap::pairing_heap<PartialSolution, boost::heap::compare<compare_ps>>;

struct cbsStat {
  double ttb;
  unsigned itb;

  cbsStat() : ttb(0.0), itb(0) {}
};

cbfsOptions opt;

void local_search(PartialSolution &s) {
  while (s.step())
    ;
}

PartialSolution construct(const Graph &g) {
  PartialSolution bco(g);

  for (auto i = 0u; i != opt.nsamples; i++) {
    PartialSolution s(g);

    unsigned tncc = i == 0 ? 1 : opt.nc;
    s.complete_greedy(tncc);

    if (s.value() < bco.value())
      bco = s;

    if (opt.improve) {
      local_search(s);
      if (s.value() < bco.value())
        bco = s;
    }
  }
  return bco;
}

pair<PartialSolution, cbsStat> cbfs(const Graph &g, string iname, timer &time) {
  unsigned processed = 0, solutions = 0, optsolutions = 1, passes = 0;
  cbsStat stat;

  vector<Heap> Q(g.nLabels);
  unsigned cdepth = 0, stored = 1;
  Q[0].emplace(g);
  PartialSolution B = construct(g);
  stat.ttb = time.elapsed();
  stat.itb = 0;

  if (B.value() == 1)
    goto done;

  while (true) {

    if (passes > opt.maxpasses)
      break;


    processed++;
    PartialSolution S(Q[cdepth].top());
    Q[cdepth].pop();
    stored--;

    unsigned mincol = 0;
    for (unsigned i = 0; i != S.ncol; ++i)
      mincol = max(mincol, S.col[i] + 1);

    for (unsigned i = S.ncol, ie = S.col.size(); i != ie; ++i) {
      if (S.col[i] < mincol)
        continue;
      PartialSolution T(S);
      T.commitColorAt(i);

      if (cdepth + 1 == g.nLabels) {
        solutions++;
        if (T.value() < B.value()) {
          optsolutions = 1;
          B = T;
          if (opt.improve)
            while (B.step())
              ;
          stat.ttb = time.elapsed();
          stat.itb = passes;
          if (B.value() == 1)
            goto done;
        } else if (T.value() == B.value())
          optsolutions++;
      } else {
        Q[cdepth + 1].emplace(T);
        if (Q[cdepth + 1].size() > 2 * opt.truncate) {
          Heap Q_ = Q[cdepth + 1];
          Q[cdepth + 1].clear();
          for (auto i = 0u; i != opt.truncate; ++i) {
            Q[cdepth + 1].emplace(Q_.top());
            Q_.pop();
          }
        }
        stored++;
      }
    }
    cdepth = cdepth + 1 < g.nLabels ? cdepth + 1 : 0;
    if (cdepth == 0)
      passes++;
    while (Q[cdepth].size() == 0 && cdepth < g.nLabels)
      cdepth++;
    if (cdepth == g.nLabels && stored > 0) {
      cdepth = 0;
      while (Q[cdepth].size() == 0 && cdepth < g.nLabels)
        cdepth++;
    }
    if (cdepth == g.nLabels)
      break;
  }
done:

  ofstream sol(fmt::format("{}.best", iname));
  fmt::print(sol, "{} {} {}\n", iname, B.to_string(), B.value());
  sol.close();

  return {B, stat};
}

int main(int argc, char *argv[]) {
  standard_description desc("General options", opt);
  cbfs_description mh("CBFS options", opt);

  desc.add(mh);
  po::positional_options_description pod;
  pod.add("instance", 1);

  po::variables_map vm;
  try {
    po::store(po::command_line_parser(argc, argv).options(desc).positional(pod).run(), vm);
    po::notify(vm);
  } catch (boost::program_options::required_option &e) {
    if (!vm.count("help"))
      fmt::print("Error: {}.\n", e.what());
    cout << desc << endl;
    return 1;
  }

  opt.seed = setupRandom(opt.seed);
  opt.iname = fs::path(opt.instance).stem().string();

  if (opt.truncate == 0)
    opt.truncate = numeric_limits<unsigned>::max();

  if (opt.maxpasses == 0)
    opt.maxpasses = numeric_limits<unsigned>::max();
  else
    opt.truncate = opt.maxpasses;

  Graph g(opt.instance);

  if (g.nLabels == 0)
    opt.iname += fs::path(opt.instance).extension().string();
  unsigned replicate = get_replicate(g, opt.iname);
  if (g.nLabels == 0)
    determine_k(g);

  opt.maxpasses = opt.truncate = opt.nsamples = opt.iterfactor * log_binomial(g.nColors, g.nLabels);

  const string iparam = fmt::format("{} {} {} {} {}", g.nNodes, g.nEdges, g.nColors, g.nLabels, replicate);

  timer tcbfs;
  auto [bs, st] = cbfs(g, opt.iname, tcbfs);

  fmt::print("CBFS {} {} {} {} {} {} {}\n", iparam, opt.truncate, opt.maxpasses, st.ttb, st.itb, bs.value(), tcbfs.elapsed());
}
