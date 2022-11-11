// Copyright (c) 2022 CODESOLE
// ini-parser.hh: ini-parser declaration & implementation file
// SPDX-License-Identifier: MIT

#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>
#include <utility>

namespace ini {

enum class comment_char : unsigned char { hash_tag = 0, semi_col = 1 };

using parsed_data =
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string>>;

template <comment_char cc = comment_char::semi_col> class parser final {
public:
  parser() = delete;
  parser(const std::string_view &in_file);
  parser(parser &&) noexcept;
  parser(const parser &) = delete;
  parser &operator=(parser &&) noexcept;
  parser &operator=(const parser &) = delete;
  ~parser() = default;

  inline void print_raw_string(void) const noexcept;
  inline const std::string &get_raw_string(void) const noexcept;
  void write_to_file(const std::string &filename) const;
  void write_to_stringstream(std::stringstream &ss);
  template <comment_char c>
  friend std::ostream &operator<<(std::ostream &os, const parser<c> &p);

  [[nodiscard("Parsed data should be used!")]] inline const parsed_data &
  get_parsed_data(void) const noexcept;

  [[nodiscard("Parsed data should be used!")]] inline parsed_data &
  get_parsed_data(void) noexcept;

  [[nodiscard("Section should be used!")]] const std::optional<
      std::reference_wrapper<
          const std::unordered_map<std::string, std::string>>>
  get_properties_of_section(const std::string &section_name) const;

  [[nodiscard("Section should be used!")]] std::optional<
      std::reference_wrapper<std::unordered_map<std::string, std::string>>>
  get_properties_of_section(const std::string &section_name);

private:
  bool is_root = true;
  std::string curr_section{"root"};
  std::ifstream input_file;
  std::string file_data;
  std::uint16_t longest_key_width; // for pretty_print function to adjust width
  std::unique_ptr<parsed_data> _parsed_data;

  static void trim_line(std::string &line);
  void get_section(std::string line, parsed_data &pd);
  std::optional<std::uint16_t> get_property(std::string line, parsed_data &pd);
  [[nodiscard("You have to check file extension!")]] static inline bool
  check_file_extension(const std::string_view &file);
};

template <comment_char c>
std::ostream &operator<<(std::ostream &os, const parser<c> &p) {
  if (p._parsed_data->contains("root")) {
    const auto &root = p._parsed_data->at("root");
    for (const auto &[key, val] : root) {
      os << '\t' << std::setw(p.longest_key_width) << key << " = " << val
         << std::endl;
    }
    os << std::endl;
  }

  for (const auto &[sections, properties] : *p._parsed_data) {
    if (sections == "root")
      continue;

    os << '[' << sections << ']' << std::endl;
    for (const auto &[key, val] : properties) {
      os << '\t' << std::setw(p.longest_key_width) << key << " = " << val
         << std::endl;
    }
    os << std::endl;
  }
  return os;
}

template <comment_char cm>
inline void parser<cm>::print_raw_string() const noexcept {
  std::cout << this->file_data << std::endl;
}

template <comment_char cm>
inline const parsed_data &parser<cm>::get_parsed_data(void) const noexcept {
  return *this->_parsed_data;
}

template <comment_char cm>
inline parsed_data &parser<cm>::get_parsed_data(void) noexcept {
  return *this->_parsed_data;
}

template <comment_char cm>
inline const std::string &parser<cm>::get_raw_string(void) const noexcept {
  return this->file_data;
}

template <comment_char cm>
inline bool parser<cm>::check_file_extension(const std::string_view &file) {
  return file.ends_with(".ini");
}

template <comment_char cm>
parser<cm>::parser(parser<cm> &&other) noexcept
    : input_file(std::move(other.input_file)),
      file_data(std::move(other.file_data)),
      longest_key_width(other.longest_key_width),
      _parsed_data(std::exchange(other._parsed_data, nullptr)) {}

template <comment_char cm>
parser<cm> &parser<cm>::operator=(parser<cm> &&other) noexcept {
  if (this != &other) {
    this->input_file = std::move(other.input_file);
    this->file_data = std::move(other.file_data);
    this->longest_key_width = other.longest_key_width;
    this->_parsed_data = std::exchange(other._parsed_data, nullptr);
  }

  return *this;
}

template <comment_char cm> void parser<cm>::trim_line(std::string &line) {
  line.erase(line.begin(),
             std::find_if(line.begin(), line.end(),
                          [](unsigned char ch) { return !std::isspace(ch); }));

  line.erase(std::find_if(line.rbegin(), line.rend(),
                          [](unsigned char ch) { return !std::isspace(ch); })
                 .base(),
             line.end());
}

template <comment_char cm>
inline void parser<cm>::get_section(std::string line, parsed_data &pd) {
  if (line.starts_with('[') && line.ends_with(']')) {
    this->is_root = false;
    this->curr_section = std::string(line.begin() + 1, line.end() - 1);
    trim_line(this->curr_section);
    if (pd.contains(this->curr_section)) {
      std::cerr << "WARN: " << this->curr_section
                << " is already exist in section list! Skipping\n";
      return;
    }
  }
}

template <comment_char cm>
std::optional<std::uint16_t> parser<cm>::get_property(std::string line,
                                                      parsed_data &pd) {
  std::string::size_type equal_symbol = line.find('=');

  if (equal_symbol != std::string::npos) {
    std::string key = std::string(line.begin(), line.begin() + equal_symbol);
    trim_line(key);
    std::string val = std::string(line.begin() + equal_symbol + 1, line.end());
    trim_line(val);
    if (this->is_root) {
      if (pd[this->curr_section].contains(key)) {
        std::cerr << "WARN: " << key
                  << " is already exist in [root] section! Skipping\n";
        return std::nullopt;
      }
      pd[this->curr_section][key] = val;
      return key.size();
    } else {
      if (pd[this->curr_section].contains(key)) {
        std::cerr << "WARN: " << key << " is already exist in ["
                  << this->curr_section << "] section! Skipping\n";
        return std::nullopt;
      }
      pd[this->curr_section][key] = val;
      return key.size();
    }
  }
  return std::nullopt;
}

template <comment_char cm>
const std::optional<
    std::reference_wrapper<const std::unordered_map<std::string, std::string>>>
parser<cm>::get_properties_of_section(const std::string &section_name) const {
  for (const auto &[sections, properties] : *this->_parsed_data) {
    if (sections == section_name)
      return std::cref(this->get_parsed_data().at(section_name));
  }
  return std::nullopt;
}

template <comment_char cm>
std::optional<
    std::reference_wrapper<std::unordered_map<std::string, std::string>>>
parser<cm>::get_properties_of_section(const std::string &section_name) {
  for (auto &[sections, properties] : *this->_parsed_data) {
    if (sections == section_name)
      return std::ref(this->get_parsed_data().at(section_name));
  }
  return std::nullopt;
}

template <comment_char cm>
parser<cm>::parser(const std::string_view &in_file)
    : longest_key_width(0), _parsed_data(nullptr) {
  try {
    if (parser<cm>::check_file_extension(in_file))
      this->input_file.open(in_file.data(), std::ios::in);
    else
      throw std::runtime_error("File should be ended with .ini extension");

    std::filesystem::path file_path(in_file);
    if (std::filesystem::exists(file_path) != true &&
        std::filesystem::is_regular_file(file_path))
      throw std::runtime_error("File not exist or not regular file!");
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    std::exit(1);
  }

  this->file_data = std::string(std::filesystem::file_size(in_file), '0');
  this->input_file.read(this->file_data.data(),
                        std::filesystem::file_size(in_file));
  char comm_token;
  switch (cm) {
  case ini::comment_char::hash_tag:
    comm_token = '#';
    break;
  case ini::comment_char::semi_col:
    comm_token = ';';
    break;
  }

  std::stringstream ss{this->file_data};
  parsed_data parsed{};

  for (std::string line{}; std::getline(ss, line, '\n');) {
    std::string::size_type idx = line.find(comm_token);
    if (idx != std::string::npos) {
      line[idx] = '\0';
      line = std::string(line.c_str());
    }

    trim_line(line);
    get_section(line, parsed);

    std::optional<std::uint16_t> key_sz = get_property(line, parsed);
    if (key_sz.has_value() && key_sz.value() > this->longest_key_width)
      this->longest_key_width = key_sz.value();
  }

  try {
    if (parsed.empty()) {
      throw std::runtime_error("ini file content is invalid");
    } else {
      this->_parsed_data = std::make_unique<parsed_data>(parsed);
    }
  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    std::exit(1);
  }

  this->is_root = true;
  this->curr_section = "root";
}

template <comment_char cm>
void parser<cm>::write_to_stringstream(std::stringstream &ss) {
  if (this->_parsed_data->contains("root")) {
    const auto &root = this->_parsed_data->at("root");
    for (const auto &[key, val] : root) {
      ss << key << " = " << val << std::endl;
    }
    ss << std::endl;
  }

  for (const auto &[sections, properties] : *this->_parsed_data) {
    if (sections == "root")
      continue;

    ss << '[' << sections << ']' << std::endl;
    for (const auto &[key, val] : properties) {
      ss << key << " = " << val << std::endl;
    }
    ss << std::endl;
  }
}

template <comment_char cm>
void parser<cm>::write_to_file(const std::string &filename) const {
  std::stringstream ss{};
  std::ofstream out_file{filename};

  if (this->_parsed_data->contains("root")) {
    const auto &root = this->_parsed_data->at("root");
    for (const auto &[key, val] : root) {
      ss << key << " = " << val << std::endl;
    }
    ss << std::endl;
  }

  for (const auto &[sections, properties] : *this->_parsed_data) {
    if (sections == "root")
      continue;

    ss << '[' << sections << ']' << std::endl;
    for (const auto &[key, val] : properties) {
      ss << key << " = " << val << std::endl;
    }
    ss << std::endl;
  }
  out_file << ss.rdbuf();
}

} // namespace ini
