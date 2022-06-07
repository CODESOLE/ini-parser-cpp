// Copyright (c) 2022 CODESOLE
// ini-parser.hh: ini-parser declarations
// SPDX-License-Identifier: GPL-3.0-or-later

#pragma once

#include <fstream>

namespace ini {

enum class comment_char : unsigned char { HASH_TAG = 0, SEMI_COL = 1 };

class parser {
public:
  parser() = delete;
  explicit parser(const std::string &in_file);
  explicit parser(const std::string &&in_file);
  parser(parser &&) = delete;
  parser(const parser &) = delete;
  parser &operator=(parser &&) = delete;
  parser &operator=(const parser &) = delete;
  ~parser() = default;

private:
  std::ifstream input_file;
  inline bool check_file_extension(const std::string &file);
};

} // namespace ini
