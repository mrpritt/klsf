/**
 * \file exhaust.cpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-11-05 10:43:03 ritt>
 */
#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <regex>
#include <unordered_set>
using namespace std;
namespace fs = std::filesystem;

#include "exhaust-options.hpp"
#include "solution.hpp"

standardOptions opt;

struct SolStats {
  unsigned best_value;
  unordered_set<ShortPartialSolution> best;
  unordered_map<unsigned, unsigned long> all;
  SolStats() : best_value(numeric_limits<unsigned>::max()) {}

  void visit(const PartialSolution &s) {
    const auto value = s.value();
    const auto repr = s.short_repr();
    if (value <= best_value) {
      if (best.size() > 0 && value < best_value)
        best.clear();
      best_value = value;
      best.insert(repr);
    }
    all[value]++;
  }

  void store(const Graph &g, const unsigned n) {
    if (n < g.nColors)
      opt.iname += fmt::format("-{}", n);

    ofstream bf(fmt::format("{}-best.dat", opt.iname));
    fmt::print(bf, "# best {}\n", best_value);
    for (const auto &s : best) {
      PartialSolution p(s);
      for (auto i = 0u; i != p.ncol; i++)
        fmt::print(bf, "{} ", g.cep[p.col[i]] + 1);
      fmt::print(bf, "# ranks: ");
      for (auto i = 0u; i != p.ncol; i++)
        fmt::print(bf, "{} ", p.col[i] + 1);
      fmt::print(bf, "\n");
    }
    bf.close();

    ofstream af(fmt::format("{}-all.dat", opt.iname));
    for (const auto &s : all)
      fmt::print(af, "{} {}\n", s.first, s.second);
  }
};

SolStats visitall(const Graph &g, const unsigned n) {
  SolStats stat;

  const auto t = g.nLabels;
  const auto t_odd = t % 2 == 1;
  
  vector<unsigned> c(t + 2, 0);
  iota(c.begin() + 1, c.end(), 0);
  c[t + 1] = n;
  
R2: {
  PartialSolution s(g);
  for (auto i = 1u; i != t + 1; ++i) {
    auto p = find(s.col.begin(), s.col.end(), c[i]);
    s.commitColorAt(p - s.col.begin());
  }
  stat.visit(s);
}

  auto j = 2u;
  if (t_odd) {
    if (c[1] + 1 < c[2]) {
      c[1]++;
      goto R2;
    }
  } else {
    if (c[1] > 0) {
      c[1]--;
      goto R2;
    }
    goto R5;
  }

R4:
  if (c[j] >= j) {
    c[j]--;
    c[j - 1] = j - 2;
    goto R2;
  }
  j++;

R5:
  if (c[j] + 1 < c[j + 1]) {
    c[j - 1] = c[j];
    c[j]++;
    goto R2;
  }
  j++;
  if (j <= t)
    goto R4;

  stat.store(g, n);
  return stat;
}

int main(int argc, char *argv[]) {
  standard_description desc("Standard options", opt);

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

  opt.iname = fs::path(opt.instance).stem().string();
  
  Graph g(opt.instance);
  
  if (g.nLabels == 0)
    opt.iname += fs::path(opt.instance).extension().string();
  unsigned replicate = get_replicate(g, opt.iname);
  if (g.nLabels == 0)
    determine_k(g);
  if (opt.k_override != 0)
    g.nLabels = opt.k_override;

  unsigned ncolors = g.nColors;
  if (vm.count("maxcolors"))
    ncolors = opt.maxcolors;
  else if (vm.count("colorfactor"))
    ncolors = ceil(g.nLabels * opt.colorfactor);

  timer tvisitall;
  auto stat = visitall(g, ncolors);

  const string iparam = fmt::format("{} {} {} {} {}", g.nNodes, g.nEdges, g.nColors, g.nLabels, replicate);
  fmt::print("EXH {} {} {} {} {}\n", iparam, ncolors, stat.best.size(), stat.best_value, tvisitall.elapsed());
}
