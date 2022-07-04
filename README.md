# ini-parser-cpp
Single header-only INI parser written in Modern C++20.

 - Same sections will be merge with a warning written into a stderr.

 - Same keys inside same sections will be skipped with warning written into stderr (only the first occurence will be chosen, others skipped).

 - However same keys in different sections are allowed.

You can find this example file in `example/main_semi.cc`. This is the whole API.

```cpp
// Copyright (c) 2022 CODESOLE
// main.cc: test file
// SPDX-License-Identifier: MIT

#include "ini-parser.hh"

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  // create ini::parser object with specified file location and with selected
  // comment-char which is can be either ';' or '#'. It will ignore contents
  // after that symbol. if you want to use comment-char inside properties you
  // can define them with
  // \; or \# to tell the parser to not see them as a comment line
  // (e.g. format=jpg\;png\;gif ) see test/sample.ini file
  // ini::comment_char::hash_tag => '#'
  // ini::comment_char::semi_col => ';'
  ini::parser<ini::comment_char::semi_col> ini_file("test/sample_semi.ini");
  //ini::parser<ini::comment_char::hash_tag> ini_file("test/sample_hash.ini");

  // print raw string that read from file
  ini_file.print_raw_string();

  // print directly into stdout with std::cout overload
  std::cout << ini_file;

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

  // dump parsed data into stringstream
  std::stringstream ss{};
  ini_file.write_to_stringstream(ss);

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
```

# LICENSE
MIT
