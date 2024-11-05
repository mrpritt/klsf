#pragma once

#include <boost/program_options.hpp>
namespace po = boost::program_options;

unsigned get_terminal_width();

struct standardOptions {
  std::string instance;
  unsigned k_override;
  std::string output;
  std::string description;
  unsigned maxcolors;
  double colorfactor;
  std::string iname;
};

struct standard_description : public boost::program_options::options_description {
  typedef boost::program_options::options_description Base;
  standard_description(const std::string &caption, standardOptions &);
};
void add_standard_options(boost::program_options::options_description &, standardOptions &);
