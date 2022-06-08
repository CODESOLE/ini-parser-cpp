// Copyright (c) 2022 CODESOLE
// ini-parser.cc: ini-parser definitions
// SPDX-License-Identifier: GPL-3.0-or-later

#include "ini-parser.hh"
#include <filesystem>
#include <iostream>

using namespace ini;

constexpr inline bool parser::check_file_extension(const std::string &file) {
  return file.ends_with(".ini");
}

void parser::print_raw_string() const { std::cout << _file_data << std::endl; }

parser::parser(const std::string &in_file)
    : _file_data(std::filesystem::file_size(in_file), '0') {
  try {
    if (check_file_extension(in_file) == true)
      this->_input_file.open(in_file, std::ios::in);
    else
      throw std::runtime_error("File should be ended with .ini extension");

    std::filesystem::path file_path(in_file);
    if (std::filesystem::exists(file_path) != true &&
        std::filesystem::is_regular_file(file_path))
      throw std::runtime_error("File not exist or not regular file!");

    this->_input_file.read(_file_data.data(),
                           std::filesystem::file_size(in_file));
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  }
}

parser::parser(const std::string &&in_file)
    : _file_data(std::filesystem::file_size(in_file), '0') {
  try {
    if (check_file_extension(std::move(in_file)) == true)
      this->_input_file.open(in_file, std::ios::in);
    else
      throw std::runtime_error("File should be ended with .ini extension");

    std::filesystem::path file_path(in_file);
    if (std::filesystem::exists(file_path) != true &&
        std::filesystem::is_regular_file(file_path) != true)
      throw std::runtime_error("File not exist or not regular file!");

    this->_input_file.read(_file_data.data(),
                           std::filesystem::file_size(in_file));
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  }
}

// static enum class

std::unique_ptr<parsed_data> parse_ini(comment_char comment_token) {
  char comm_token = static_cast<bool>(comment_token) ? ';' : '#';
  std::unique_ptr<parsed_data> parsed = std::make_unique<parsed_data>();
  return parsed;
}
