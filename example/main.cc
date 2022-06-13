// Copyright (c) 2022 CODESOLE
// main.cc: test file
// SPDX-License-Identifier: MIT

#include "ini-parser.hh"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  // create ini::parser object with specified file location
  ini::parser ini_file = ini::parser("tests/sample.ini");

  // after opening file parse with selected comment-char which is can be
  // either ';' or '#'. It will ignore contents after specified symbol.
  // if you want to use comment-char inside properties you can define them with
  // \; or \# to tell the parser to not see them as a comment line.
  ini_file.parse_ini(ini::comment_char::SEMI_COL);

  // print raw string that read from file
  ini_file.print_raw_string();

  // print parsed elements in a pretty way
  ini_file.pretty_print();

  // get raw string that read from file
  const auto &raw_str = ini_file.get_raw_string();
  std::cout << raw_str << std::endl;

  // get const reference of parsed data structure which is
  // std::unordered_map<std::string, std::unordered_map<std::string,
  // std::string>>
  const ini::parsed_data &const_ref = ini_file.get_parsed_data();

  // get non-const reference of parsed data structure which is
  // std::unordered_map<std::string, std::unordered_map<std::string,
  // std::string>>
  ini::parsed_data &non_const_ref = ini_file.get_parsed_data();

  // accessing at specific section and specific property (e.g http -> port)
  std::cout << const_ref.at("http").at("port") << std::endl;

  // modifying parsed element
  non_const_ref.at("http").at("port") = "3443";
  std::cout << const_ref.at("http").at("port") << std::endl;

  // write parsed/modified data structure into a new .ini file
  ini_file.write_to_file("with_new_edited_port.ini");

  // const reference
  if (ini_file.get_properties_of_section("http").has_value()) {
    const auto &const_http_section =
        ini_file.get_properties_of_section("http").value().get();
    for (const auto &[key, val] : const_http_section)
      std::cout << key << " = " << val << std::endl;
  }

  // non-const reference
  if (ini_file.get_properties_of_section("http").has_value()) {
    auto &non_const_http_section =
        ini_file.get_properties_of_section("http").value().get();
    for (auto &[key, val] : non_const_http_section)
      std::cout << key << " = " << val << std::endl;
  }

  return 0;
}
