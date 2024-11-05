/**
 * \file random.hpp
 *   \author Marcus Ritt <marcus.ritt@inf.ufrgs.br>
 *   \version $Id: emacs 7968 2017-05-17 23:03:17Z ritt $
 *   \date Time-stamp: <2024-11-04 12:12:33 ritt>
 */
#pragma once

#include <random>

extern std::mt19937 rng;

unsigned setupRandom(unsigned seed = 0);
int getRandom(int min, int max);
