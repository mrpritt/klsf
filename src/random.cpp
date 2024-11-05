/**
 * \file random.cpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \version $Id: emacs 7968 2017-05-17 23:03:17Z ritt $
 *   \date Time-stamp: <2024-11-04 12:12:22 ritt>
 */
#include "random.hpp"

#include <fstream>
using namespace std;

mt19937 rng;

unsigned setupRandom(unsigned seed) {
  if (seed == 0) {
    seed = time(0);
    ifstream f("/dev/urandom");
    if (f.good()) {
      f.read((char *)(&seed), sizeof(unsigned int));
    }
  }
  rng.seed(seed);
  srand48(seed);
  return seed;
}

int getRandom(int min, int max) {
  uniform_int_distribution<> U(min, max);
  return U(rng);
}
