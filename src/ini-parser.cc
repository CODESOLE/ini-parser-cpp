// Copyright (c) 2022 CODESOLE
// ini-parser.cc: ini-parser definitions
// SPDX-License-Identifier: MIT

#include "ini-parser.hh"
#include <algorithm>
#include <filesystem>

using namespace ini;

inline bool parser::check_file_extension(const std::string_view &file) {
  return file.ends_with(".ini");
}

parser::parser(parser &&other)
    : _input_file(std::move(other._input_file)),
      _file_data(std::move(other._file_data)),
      longest_key_width(other.longest_key_width),
      _parsed_data(std::move(other._parsed_data)) {
  other._parsed_data = nullptr;
}

parser &parser::operator=(parser &&other) {
  this->_input_file = std::move(other._input_file);
  this->_file_data = std::move(other._file_data);
  this->longest_key_width = other.longest_key_width;
  this->_parsed_data = std::move(other._parsed_data);
  other._parsed_data = nullptr;
  return *this;
}

parser::parser(const std::string_view &in_file)
    : _file_data(std::filesystem::file_size(in_file), '0'),
      longest_key_width(0), _parsed_data(nullptr) {
  try {
    if (check_file_extension(in_file))
      this->_input_file.open(in_file.data(), std::ios::in);
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

constinit bool is_root = true;

static std::string curr_section{};

static void trim_line(std::string &line) {
  line.erase(line.begin(),
             std::find_if(line.begin(), line.end(),
                          [](unsigned char ch) { return !std::isspace(ch); }));

  line.erase(std::find_if(line.rbegin(), line.rend(),
                          [](unsigned char ch) { return !std::isspace(ch); })
                 .base(),
             line.end());
}

static inline void get_section(std::string line, parsed_data &pd) {
  if (*line.begin() == '[' && *(line.end() - 1) == ']') {
    is_root = false;
    curr_section = std::string(line.begin() + 1, line.end() - 1);
    if (pd.contains(curr_section)) {
      std::cerr << "WARN: " << curr_section
                << " is exist in section list! Skipping\n";
      return;
    }
    trim_line(curr_section);
  }
}

static std::optional<std::uint16_t> get_property(std::string line,
                                                 parsed_data &pd) {
  std::string::size_type equal_symbol = line.find("=");

  if (equal_symbol != std::string::npos) {
    std::string key = std::string(line.begin(), line.begin() + equal_symbol);
    trim_line(key);
    std::string val = std::string(line.begin() + equal_symbol + 1, line.end());
    trim_line(val);
    if (is_root) {
      if (pd["root"].contains(key)) {
        std::cerr << "WARN: " << key
                  << " is exist in [root] section! Skipping\n";
        return {};
      }
      pd["root"][key] = val;
      return key.size();
    } else {
      if (pd[curr_section].contains(key)) {
        std::cerr << "WARN: " << key << " is exist in [" << curr_section
                  << "] section! Skipping\n";
        return {};
      }
      pd[curr_section][key] = val;
      return key.size();
    }
  }
  return {};
}

void parser::parse_ini(comment_char comment_token = comment_char::SEMI_COL) {
  char comm_token;
  switch (comment_token) {
  case ini::comment_char::HASH_TAG:
    comm_token = '#';
    break;
  case ini::comment_char::SEMI_COL:
    comm_token = ';';
    break;
  }

  std::stringstream ss{_file_data};
  parsed_data parsed{};

  for (std::string line{}; std::getline(ss, line, '\n');) {
    std::string::size_type idx = line.find(comm_token);
    if (idx != std::string::npos) {
      if (line[idx - 1] != '\\') {
        line[idx] = '\0';
        line = std::string(line.c_str());
      }
      line.erase(std::remove(line.begin(), line.end(), '\\'), line.end());
    }

    trim_line(line);
    get_section(line, parsed);

    std::optional<std::uint16_t> key_sz = get_property(line, parsed);
    if (key_sz.has_value() && key_sz > this->longest_key_width)
      this->longest_key_width = key_sz.value();
  }
  this->_parsed_data = std::make_unique<parsed_data>(parsed);
  is_root = true;
  curr_section.clear();
}

const std::optional<
    std::reference_wrapper<const std::unordered_map<std::string, std::string>>>
parser::get_properties_of_section(const std::string &section_name) const {
  for (auto &[sections, properties] : *this->_parsed_data) {
    if (sections == section_name)
      return this->get_parsed_data().at(section_name);
  }
  return {};
}

std::optional<
    std::reference_wrapper<std::unordered_map<std::string, std::string>>>
parser::get_properties_of_section(const std::string &section_name) {
  for (auto &[sections, properties] : *this->_parsed_data) {
    if (sections == section_name)
      return this->get_parsed_data().at(section_name);
  }
  return {};
}

void parser::pretty_print(void) const noexcept {
  for (auto &[sections, properties] : *this->_parsed_data) {
    std::cout << '[' << sections << ']' << std::endl;
    for (auto &[key, val] : properties) {
      std::cout << '\t' << std::setw(this->longest_key_width) << key << " = "
                << val << std::endl;
    }
    std::cout << std::endl;
  }
}

void parser::write_to_file(const std::string &filename) {
  std::stringstream ss{};
  std::ofstream out_file{filename};
  for (auto &[sections, properties] : *this->_parsed_data) {
    ss << '[' << sections << ']' << std::endl;
    for (auto &[key, val] : properties) {
      ss << key << " = " << val << std::endl;
    }
    ss << std::endl;
  }
  out_file << ss.rdbuf();
}
