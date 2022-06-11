// Copyright (c) 2022 CODESOLE
// main.cc: entry point
// SPDX-License-Identifier: MIT

#include "ini-parser.hh"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  ini::parser ini_file = ini::parser("tests/sample.ini");
  // ini_file.print_raw_string();
  // const auto &raw_str = ini_file.get_raw_string();
  // std::cout << raw_str << std::endl;
  ini_file.parse_ini(ini::comment_char::SEMI_COL);
  const ini::parsed_data &x = ini_file.get_parsed_data();
  std::cout << x.at("database]").at("driverclass") << std::endl;

  return 0;
}
