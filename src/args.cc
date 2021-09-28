#include <algorithm>
#include <args.h>
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#define __flag_type_bool 0
#define __flag_type_int 1
#define __flag_type_double 2
#define __flag_type_uint64 3
#define __flag_type_string 4

struct __flag_item {
  uint8_t type;
  char short_name;
  std::string full_name;
  std::string description;
  void *value;
};

std::vector<__flag_item> __flag_items = {};

#define flag_bool(var_name, default_value, flag_short_name, flag_full_name,    \
                  flag_description)                                            \
  var_name = default_value;                                               \
  __flag_items.push_back({__flag_type_bool, flag_short_name, flag_full_name,   \
                          flag_description, &var_name})

#define flag_int(var_name, default_value, flag_short_name, flag_full_name,     \
                 flag_description)                                             \
  var_name = (int)default_value;                                                \
  __flag_items.push_back({__flag_type_int, flag_short_name, flag_full_name,    \
                          flag_description, &var_name})

#define flag_double(var_name, default_value, flag_short_name, flag_full_name,  \
                    flag_description)                                          \
  var_name = (double)default_value;                                             \
  __flag_items.push_back({__flag_type_double, flag_short_name, flag_full_name, \
                          flag_description, &var_name})

#define flag_uint64(var_name, default_value, flag_short_name, flag_full_name,  \
                    flag_description)                                          \
  var_name = (uint64_t)default_value;                                           \
  __flag_items.push_back({__flag_type_uint64, flag_short_name, flag_full_name, \
                          flag_description, &var_name})

#define flag_string(var_name, default_value, flag_short_name, flag_full_name,  \
                    flag_description)                                          \
  var_name = std::string(default_value);                                        \
  __flag_items.push_back({__flag_type_string, flag_short_name, flag_full_name, \
                          flag_description, &var_name})

std::vector<std::string> __flag_args;

void __flag_set_int(std::vector<__flag_item>::iterator &ex, std::string &value,
                    char *arg) {
  try {
    int *v = (int *)ex->value;
    if (value[0] == '0' && (value[1] == 'x' || value[1] == 'X'))
      *v = std::stoi(value.substr(2), nullptr, 16);
    else if (value[0] == '0' && (value[1] == 'o' || value[1] == 'O'))
      *v = std::stoi(value.substr(2), nullptr, 8);
    else if (value[0] == '0' && (value[1] == 'b' || value[1] == 'B'))
      *v = std::stoi(value.substr(2), nullptr, 2);
    else
      *v = std::stoi(value);
  } catch (const std::exception &) {
    std::cerr << "Invalid argument: " << arg << " " << value << std::endl;
    std::exit(0);
  }
}

void __flag_set_double(std::vector<__flag_item>::iterator &ex,
                       std::string &value, char *arg) {
  try {
    double *v = (double *)ex->value;
    *v = std::stod(value);
  } catch (const std::exception &) {
    std::cerr << "Invalid argument: " << arg << " " << value << std::endl;
    std::exit(0);
  }
}

void __flag_set_uint64(std::vector<__flag_item>::iterator &ex,
                       std::string &value, char *arg) {
  try {
    uint64_t *v = (uint64_t *)ex->value;
    if (value[0] == '0' && (value[1] == 'x' || value[1] == 'X'))
      *v = std::stoull(value.substr(2), nullptr, 16);
    else if (value[0] == '0' && (value[1] == 'o' || value[1] == 'O'))
      *v = std::stoull(value.substr(2), nullptr, 8);
    else if (value[0] == '0' && (value[1] == 'b' || value[1] == 'B'))
      *v = std::stoull(value.substr(2), nullptr, 2);
    else
      *v = std::stoull(value);
  } catch (const std::exception &) {
    std::cerr << "Invalid argument: " << arg << " " << value << std::endl;
    std::exit(0);
  }
}

void flag_parse(int argc, char **argv) {
  for (size_t i = 1; i < argc; ++i) {
    auto len = std::strlen(argv[i]);
    bool next_value_used = false;
    if (len > 1 && argv[i][0] == '-') {
      if (argv[i][1] == '-') {
        std::stringstream ss(argv[i]);
        std::string key;
        std::getline(ss, key, '=');
        key = key.substr(2);
        std::string value;
        std::getline(ss, value);
        auto ex =
            std::find_if(__flag_items.begin(), __flag_items.end(),
                         [key](__flag_item &i) { return i.full_name == key; });
        if (ex == __flag_items.end()) {
          std::cerr << "Invalid argument: " << argv[i] << std::endl;
          std::exit(0);
        }

        if (ex->type != __flag_type_bool && !value.size()) {
          auto j = i + 1;
          if (j == argc) {
            std::cerr << "Invalid argument: " << argv[i] << std::endl;
            std::exit(0);
          } else {
            value = std::string(argv[j]);
            next_value_used = true;
          }
        }

        switch (ex->type) {
        case __flag_type_bool: {
          if (value.size()) {
            std::transform(value.begin(), value.end(), value.begin(),
                           [](unsigned char c) { return std::tolower(c); });
            if (value == "true" || value == "1") {
              *(bool *)ex->value = true;
            } else if (value == "false" || value == "0") {
              *(bool *)ex->value = false;
            } else {
              std::cerr << "Invalid argument: " << argv[i] << std::endl;
              std::exit(0);
            }
          } else {
            *(bool *)ex->value = true;
          }
        } break;
        case __flag_type_int: {
          __flag_set_int(ex, value, argv[i]);
        } break;
        case __flag_type_double: {
          __flag_set_double(ex, value, argv[i]);
        } break;
        case __flag_type_uint64: {
          __flag_set_uint64(ex, value, argv[i]);
        } break;
        case __flag_type_string: {
          *(std::string *)ex->value = value;
        } break;
        }

      } else {
        for (size_t j = 1; j < len; ++j) {
          auto &c = argv[i][j];
          if (c < 'A' || c > 'z' || (c > 'Z' && c < 'a')) {
            std::cerr << "Invalid argument: " << argv[i] << std::endl;
            std::exit(0);
          }
          auto ex =
              std::find_if(__flag_items.begin(), __flag_items.end(),
                           [c](__flag_item &i) { return i.short_name == c; });
          if (ex == __flag_items.end()) {
            std::cerr << "Invalid argument: " << argv[i] << std::endl;
            std::exit(0);
          }

          if (ex->type == __flag_type_bool) {
            *(bool *)ex->value = true;
          } else {
            next_value_used = true;
            auto j = i + 1;
            if (j == argc) {
              std::cerr << "Invalid argument: " << argv[i] << std::endl;
              std::exit(0);
            }
            auto value = std::string(argv[j]);
            switch (ex->type) {
            case __flag_type_int: {
              __flag_set_int(ex, value, argv[i]);
            } break;
            case __flag_type_double: {
              __flag_set_double(ex, value, argv[i]);
            } break;
            case __flag_type_uint64: {
              __flag_set_uint64(ex, value, argv[i]);
            } break;
            case __flag_type_string: {
              *(std::string *)ex->value = value;
            } break;
            }
          }
        }
      }
    } else {
      __flag_args.push_back(argv[i]);
    }
    if (next_value_used)
      ++i;
  }
}

int main(int argc, char **argv) {
  bool extra;
  auto flag_bool(verbose, false, 'v', "verbose", "");
  flag_bool(extra, true, 'x', "extra", "");
  auto flag_int(number, 3, 'n', "num", "");
  auto flag_double(dv, 1.1, 'd', "double", "");
  auto flag_uint64(u64, 2, 'u', "uint64", "");
  auto flag_string(s, "xx", 's', "ss", "");
  flag_parse(argc, argv);
  for (auto arg : __flag_args)
    std::cout << arg << std::endl;

  std::cout << "Verbose mode is " << (verbose ? "ON" : "OFF") << std::endl;
  std::cout << "Extra mode is " << (extra ? "ON" : "OFF") << std::endl;
  std::cout << "n: " << number << std::endl;
  std::cout << "d: " << dv << std::endl;
  std::cout << "u: " << u64 << std::endl;
  std::cout << "s: " << s << std::endl;
}