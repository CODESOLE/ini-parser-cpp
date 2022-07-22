// Copyright (c) 2022 CODESOLE
// main.cc: test file
// SPDX-License-Identifier: MIT

#include "ini-parser.hh"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  // create ini::parser object with specified file location and with selected
  // comment-char which can be either ';' or '#'. It will ignore contents
  // after that symbol.
  // ini::comment_char::HASH_TAG => '#'
  // ini::comment_char::SEMI_COL => ';'
  ini::parser<ini::comment_char::semi_col> ini_file("test/sample_semi.ini");
  // ini::parser<ini::comment_char::hash_tag> ini_file("test/sample_hash.ini");

  std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
  std::cout << "0)PRINT DIRECTLY INTO STDOUT WITH STD::COUT <<" << std::endl;
  std::cout << ini_file;
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
            << std::endl
            << std::endl
            << std::endl
            << std::endl;

  std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
  std::cout << "1)PRINT RAW STRING" << std::endl;
  // print raw string that read from file
  ini_file.print_raw_string();
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
            << std::endl
            << std::endl
            << std::endl
            << std::endl;

  // get raw string that read from file
  [[maybe_unused]] const auto &raw_str = ini_file.get_raw_string();
  // std::cout << raw_str << std::endl;

  // get const reference of parsed data structure which is
  // std::unordered_map<std::string, std::unordered_map<std::string,
  // std::string>>
  const ini::parsed_data &const_ref = ini_file.get_parsed_data();

  // get non-const reference of parsed data structure which is
  // std::unordered_map<std::string, std::unordered_map<std::string,
  // std::string>>
  ini::parsed_data &non_const_ref = ini_file.get_parsed_data();

  std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
  std::cout << "2)ACCESSING SPECIFIC SECTION -> PROPERTY" << std::endl;
  // accessing at specific section and specific property (e.g http -> port)
  std::cout << const_ref.at("http").at("port") << std::endl;
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
            << std::endl
            << std::endl
            << std::endl
            << std::endl;

  std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
  std::cout << "3)MODIFYING SPECIFIC SECTION -> PROPERTY" << std::endl;
  // modifying parsed element
  non_const_ref.at("http").at("port") = "3443";
  std::cout << const_ref.at("http").at("port") << std::endl;
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
            << std::endl
            << std::endl
            << std::endl
            << std::endl;

  // write parsed/modified data structure into a new .ini file
  ini_file.write_to_file("with_new_edited_port.ini");

  std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
  std::cout << "4)get_properties_of_section (const version)" << std::endl;
  // const reference
  if (ini_file.get_properties_of_section("http").has_value()) {
    const auto &const_http_section =
        ini_file.get_properties_of_section("http").value().get();
    for (const auto &[key, val] : const_http_section)
      std::cout << key << " = " << val << std::endl;
  }
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
            << std::endl
            << std::endl
            << std::endl
            << std::endl;

  std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
  std::cout << "5)get_properties_of_section (non-const version)" << std::endl;
  // non-const reference
  if (ini_file.get_properties_of_section("http").has_value()) {
    auto &non_const_http_section =
        ini_file.get_properties_of_section("http").value().get();
    for (auto &[key, val] : non_const_http_section)
      std::cout << key << " = " << val << std::endl;
  }
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
            << std::endl
            << std::endl
            << std::endl
            << std::endl;

  std::cout << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
  std::cout << "6)DUMP PARSED DATA INTO STRINGSTREAM" << std::endl;
  // write parsed data into stringstream
  std::stringstream ss{};
  ini_file.write_to_stringstream(ss);
  std::cout << ss.rdbuf();
  std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
            << std::endl
            << std::endl
            << std::endl
            << std::endl;

  return 0;
}
