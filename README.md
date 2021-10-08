## C++ args
```c++
#include <args.h>

int main(int argc, char **argv) {
  bool extra;
  auto flag_bool(verbose, false, 'v', "verbose", "xxxxxxxxxxxxx");
  flag_bool(extra, true, 'x', "extra", "xxxxxxxxxxxxx");
  auto flag_int(number, 3, 'n', "num", "xxxxxxxxxxxxx");
  auto flag_double(dv, 1.1, 'd', "double", "xxxxxxxxxxxxx");
  auto flag_uint64(u64, 2, 'u', "", "xxxxxxxxxxxxx");
  auto flag_string(s, "xx", ' ', "ss", "xxxxxxxxxxxxx");
  auto flag_bool(h, false, 'h', "help", "xxxxxxxxxxxxx");
  flag_parse(argc, argv);
  auto help = flag_help("Usage: exe [options] <output>");
  if (h) {
    std::cout << help;
    std::exit(0);
  }
  for (auto arg : flag_args)
    std::cout << arg << std::endl;

  std::cout << "Verbose mode is " << (verbose ? "ON" : "OFF") << std::endl;
  std::cout << "Extra mode is " << (extra ? "ON" : "OFF") << std::endl;
  std::cout << "n: " << number << std::endl;
  std::cout << "d: " << dv << std::endl;
  std::cout << "u: " << u64 << std::endl;
  std::cout << "s: " << s << std::endl;
}

```
