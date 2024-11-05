#include "cbfs-options.hpp"

#include <cstdlib>
#include <string>
#include <vector>
#include <sys/ioctl.h>

#include "logging.hpp"
using namespace logging;

unsigned get_terminal_width() {
  struct winsize wsize;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &wsize);
  return wsize.ws_col;
}

void add_standard_options(po::options_description &desc, standardOptions &opt) {
  desc.add_options()
  ("help", "Show help.")
  ("instance", po::value<std::string>(&opt.instance)->required(), "Instance.")
  ("timelimit", po::value<unsigned>(&opt.tlim)->default_value(600), "Time limit (seconds).")
  ("seed", po::value<unsigned>(&opt.seed)->default_value(1), "Random seed (0: get from random source)")
  ("output", po::value<std::string>(&opt.output), "Output directory.")
  ("description", po::value<std::string>(&opt.description)->default_value("CBFS"), "Description of the run.")
  ;
}

standard_description::standard_description(const std::string &caption, standardOptions &opt) : Base(caption, get_terminal_width()) { add_standard_options(*this, opt); }

void add_cbfs_options(po::options_description &desc, cbfsOptions &opt) {
  desc.add_options()
  ("improve", po::bool_switch(&opt.improve)->default_value(false), "Apply local search to constructed solutions.")
  ("truncate", po::value<unsigned>(&opt.truncate)->default_value(0), "Maximum number of solutions per level.")
  ("maxpasses", po::value<unsigned>(&opt.maxpasses)->default_value(0), "Maximum number of passes.")
  ("nsamples", po::value<unsigned>(&opt.nsamples)->default_value(1), "Number of samples in sampling methods.")
  ("candidates", po::value<unsigned>(&opt.nc)->default_value(3), "Number of candidates in randomized greedy construction.")
  ("iterfactor", po::value<unsigned>(&opt.iterfactor)->default_value(10), "Iterations multiplier.")
  ;
}

cbfs_description::cbfs_description(const std::string &caption, cbfsOptions &opt) : Base(caption, get_terminal_width()) { add_cbfs_options(*this, opt); }
