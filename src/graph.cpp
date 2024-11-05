/**
 * \file graph.cpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-11-05 10:44:43 ritt>
 */
#include "graph.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <queue>
#include <regex>
#include <numeric>
using namespace std;

#include "utilities.hpp"

Graph::Graph(std::string ifname) { read(ifname); }

void Graph::read(string ifname) {
  ifstream in(ifname);
  if (!in.is_open())
    throw invalid_argument("Input file does not exist.");
  read(in);
  in.close();
}

void Graph::read(istream &in) {
  read_klsf(in);
}

void Graph::read_klsf(istream &in) {
  in >> nNodes >> nEdges >> nColors >> nLabels;
  read_sparse_bottom(in);
}

void Graph::read_sparse_bottom(istream &in) {
  nIn.resize(nNodes);

  vector<unsigned> s, d, c;
  for (auto i = 0u; i != nEdges; ++i) {
    unsigned cs, cd, cc;
    in >> cs >> cd >> cc;
    cs--;
    cd--;
    cc--;
    s.push_back(cs);
    d.push_back(cd);
    c.push_back(cc);
    nIn[cs]++;
    nIn[cd]++;
  }
  build_from_vectors(s, d, c);
}

void Graph::build_from_vectors(const vector<unsigned> &s, const vector<unsigned> &d, const vector<unsigned> &c) {
  inc.resize(2 * nEdges);
  col.resize(2 * nEdges);

  inp.resize(nNodes + 1);
  inp[0] = 0;
  for (auto i = 1u; i != nNodes + 1; ++i)
    inp[i] = inp[i - 1] + nIn[i - 1];
  for (auto i = 0u; i != nEdges; ++i) {
    inc[inp[s[i]]] = d[i];
    inc[inp[d[i]]] = s[i];
    col[inp[s[i]]] = c[i];
    col[inp[d[i]]] = c[i];
    inp[s[i]]++;
    inp[d[i]]++;
  }
  inp[0] = 0;
  for (auto i = 1u; i != nNodes + 1; ++i)
    inp[i] = inp[i - 1] + nIn[i - 1];

  nEdges *= 2;

  ce.resize(nColors);
  compute_sf();

  cep.resize(nColors);
  iota(cep.begin(), cep.end(), 0);
  sort(cep.begin(), cep.end(), [&](unsigned c1, unsigned c2) { return ce[c1].size() > ce[c2].size(); });
  vector<unsigned> icep(nColors, 0);
  for (auto i = 0u; i != nColors; ++i)
    icep[cep[i]] = i;
  for (auto i = 0u; i != nEdges; ++i)
    col[i] = icep[col[i]];
  compute_sf();
}

void Graph::compute_sf() {
  for (auto i = 0u; i != nColors; ++i) {
    ce[i].clear();
    sf_color(i);
  }
}

void Graph::sf_color(const unsigned c) {
  vector<bool> visited(nNodes, false);
  for (auto j = 0u; j != nNodes; ++j)
    if (!visited[j]) {
      visited[j] = true;
      bfs_color(j, c, visited);
    }
}

void Graph::bfs_color(unsigned u, const unsigned c, vector<bool> &visited) {
  queue<unsigned> q;
  q.push(u);
  while (!q.empty()) {
    u = q.front();
    q.pop();
    for (auto k = inp[u], ke = inp[u + 1]; k != ke; ++k) {
      auto v = inc[k];
      if (col[k] == c && !visited[v]) {
        visited[v] = true;
        q.push(v);
        ce[c].push_back({u, v});
      }
    }
  }
}

unsigned Graph::num_edges() const {
  unsigned e = 0;
  for (auto i = 0u; i != nColors; ++i)
    e += ce[i].size();
  return e;
}

void applyColor(const Graph &g, unsigned col, uf &U) {
  for (const auto &[u, v] : g.ce[col])
    U.unite(u, v);
}

unsigned get_replicate(const Graph &g, string iname) {
  unsigned replicate = 0;

  string sep = g.nLabels == 0 ? "=" : "_";
  const string iparam = regex_replace(iname, regex(sep), " ");
  istringstream iss(iparam);
  iss >> ignoreToken >> ignoreToken >> ignoreToken >> ignoreToken >> replicate;
  return replicate;
}
