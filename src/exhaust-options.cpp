#include "exhaust-options.hpp"

#include <cstdlib>
#include <string>
#include <sys/ioctl.h>
#include <vector>

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
  ("k-override", po::value<unsigned>(&opt.k_override)->default_value(0), "Override k-value.")
  ("output", po::value<std::string>(&opt.output), "Output directory.")
  ("description", po::value<std::string>(&opt.description)->default_value("Exhaustive search"), "Description of the run.")
  ("maxcolors", po::value<unsigned>(&opt.maxcolors), "Maximum number of colors to consider (in order).")
  ("colorfactor", po::value<double>(&opt.colorfactor), "Maximum number of colors to consider (in order), as a fraction of the labels.")
  ;
}

standard_description::standard_description(const std::string &caption, standardOptions &opt) : Base(caption, get_terminal_width()) { add_standard_options(*this, opt); }
