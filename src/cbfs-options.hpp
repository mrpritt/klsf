#pragma once

#include <boost/program_options.hpp>
namespace po = boost::program_options;

unsigned get_terminal_width();

struct standardOptions {
  std::string instance;
  unsigned tlim;
  unsigned seed;
  std::string output;
  std::string description;
  std::string iname;
};

struct standard_description : public boost::program_options::options_description {
  typedef boost::program_options::options_description Base;
  standard_description(const std::string &caption, standardOptions &);
};
void add_standard_options(boost::program_options::options_description &, standardOptions &);

struct cbfsOptions : public standardOptions {
  bool improve;
  unsigned truncate;
  unsigned maxpasses;
  unsigned nsamples;
  unsigned nc;
  unsigned iterfactor;
};

struct cbfs_description : public boost::program_options::options_description {
  typedef boost::program_options::options_description Base;
  cbfs_description(const std::string &caption, cbfsOptions &);
};
void add_cbfs_options(boost::program_options::options_description &, cbfsOptions &);
