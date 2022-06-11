// Copyright (c) 2022 CODESOLE
// ini-parser.hh: ini-parser declarations
// SPDX-License-Identifier: MIT

#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>

namespace ini {

enum class comment_char : unsigned char { HASH_TAG = 0, SEMI_COL = 1 };

using parsed_data =
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string>>;

class parser {
public:
  parser() = delete;
  explicit parser(const std::string_view &in_file);
  parser(parser &&) = delete;
  parser(const parser &) = delete;
  parser &operator=(parser &&) = delete;
  parser &operator=(const parser &) = delete;
  ~parser() = default;

  inline void print_raw_string(void) const;
  inline const std::string &get_raw_string(void) const;
  void parse_ini(comment_char comment_token);
  [[nodiscard("Parsed data should be used!")]] inline const parsed_data &
  get_parsed_data(void) const;

private:
  std::ifstream _input_file;
  std::string _file_data;
  std::unique_ptr<parsed_data> _parsed_data = nullptr;
  [[nodiscard("You have to check file extension!")]] inline bool
  check_file_extension(const std::string_view &file);
};

inline void parser::print_raw_string() const {
  std::cout << _file_data << std::endl;
}

inline const parsed_data &parser::get_parsed_data(void) const {
  return *this->_parsed_data;
}

inline const std::string &parser::get_raw_string(void) const {
  return _file_data;
}

} // namespace ini
