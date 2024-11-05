/**
 * \file graph.hpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \date Time-stamp: <2024-11-04 12:40:02 ritt>
 */
#pragma once

#include <iostream>
#include <vector>

#include "uf.hpp"

struct Graph {
  Graph() : nNodes(0), nEdges(0), nColors(0), nLabels(0) {}
  Graph(std::string);
  unsigned nNodes, nEdges, nColors, nLabels;
  std::vector<unsigned> nIn;
  std::vector<unsigned> inc, col;
  std::vector<unsigned> inp;
  std::vector<std::vector<std::pair<unsigned, unsigned>>> ce;
  std::vector<unsigned> cep;

  void read(std::istream &);
  void read_mlst(std::istream &);
  void read_klsf(std::istream &);
  void read_sparse_bottom(std::istream &);
  bool read_dense_bottom(std::istream &in, unsigned n, unsigned L);
  void read(std::string);
  void build_from_vectors(const std::vector<unsigned> &, const std::vector<unsigned> &, const std::vector<unsigned> &);

  void compute_sf();
  void sf_color(const unsigned);
  void bfs_color(unsigned, const unsigned, std::vector<bool> &);

  unsigned num_edges() const;
};

void applyColor(const Graph &g, unsigned col, uf &U);
unsigned get_replicate(const Graph &g, std::string iname);
