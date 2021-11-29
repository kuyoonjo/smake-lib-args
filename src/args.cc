#include <args.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int argc, char **argv) {
  bool extra;
  auto flag_bool(verbose, false, 'v', "verbose", "xxxxxxxxxxxxx");
  flag_bool(extra, true, 'x', "extra", "xxxxxxxxxxxxx");
  auto flag_int(number, 3, 'n', "num", "xxxxxxxxxxxxx");
  auto flag_double(dv, 1.1, 'd', "double", "xxxxxxxxxxxxx");
  auto flag_uint64(u64, 2, 'u', "", "xxxxxxxxxxxxx");
  auto flag_string(s, "xx", ' ', "ss", "xxxxxxxxxxxxx");
  auto flag_bool(h, false, 'h', "help", "xxxxxxxxxxxxx");
  auto help = flag_help("Usage: exe [options] <output>");
  try {
    flag_parse(argc, argv);
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    std::cout << help;
    std::exit(1);
  }
  if (h) {
    std::cout << help;
    std::exit(0);
  }
  for (auto arg : flag_args)
    std::cout << arg << std::endl;

  std::cout << (verbose_is_set ? "set ": "not set ") << "Verbose mode is " << (verbose ? "ON" : "OFF") << std::endl;
  std::cout << (extra_is_set ? "set ": "not set ") << "Extra mode is " << (extra ? "ON" : "OFF") << std::endl;
  std::cout << (number_is_set ? "set ": "not set ") << "n: " << number << std::endl;
  std::cout << (dv_is_set ? "set ": "not set ") << "d: " << dv << std::endl;
  std::cout << (u64_is_set ? "set ": "not set ") << "u: " << u64 << std::endl;
  std::cout << (s_is_set ? "set ": "not set ") << "s: " << s << std::endl;
}
