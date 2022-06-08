// Copyright (c) 2022 CODESOLE
// main.cc: entry point
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ini-parser.hh"
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  ini::parser ini_file = ini::parser("tests/sample.ini");
  ini_file.print_raw_string();
  std::unique_ptr<ini::parsed_data> parsed_ini =
      ini_file.parse_ini(ini::comment_char::SEMI_COL);

  return 0;
}
