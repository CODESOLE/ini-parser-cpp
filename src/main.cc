// Copyright (c) 2022 CODESOLE
// main.cc: entry point
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ini-parser.hh"
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  ini::parser ini_file = ini::parser("sample.ini");

  return 0;
}
