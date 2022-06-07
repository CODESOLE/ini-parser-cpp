// Copyright (c) 2022 CODESOLE
// ini-parser.cc: ini-parser definitions
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ini-parser.hh"
#include <iostream>
#include <memory>

using ini::parser;

constexpr inline bool parser::check_file_extension(const std::string &file) {
  return file.ends_with(".ini");
}

parser::parser(const std::string &in_file) {
  try {
    if (check_file_extension(in_file) == true)
      this->input_file.open(in_file, std::ios::in);
    else
      throw std::runtime_error("file should be ended with .ini extension");
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  }
}

parser::parser(const std::string &&in_file) {
  try {
    if (check_file_extension(in_file) == true)
      this->input_file.open(in_file, std::ios::in);
    else
      throw std::runtime_error("File should be ended with .ini extension");
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  }
}
