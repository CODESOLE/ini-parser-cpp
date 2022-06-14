// Copyright (c) 2022 CODESOLE
// ini-parser.hh: ini-parser declarations
// SPDX-License-Identifier: MIT

#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <unordered_map>

namespace ini {

enum class comment_char : unsigned char { HASH_TAG = 0, SEMI_COL = 1 };

using parsed_data =
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::string>>;

class parser {
public:
  parser() = delete;
  explicit parser(const std::string_view &in_file, comment_char comment_char);
  parser(parser &&);
  parser(const parser &) = delete;
  parser &operator=(parser &&);
  parser &operator=(const parser &) = delete;
  ~parser() = default;

  inline void print_raw_string(void) const noexcept;
  inline const std::string &get_raw_string(void) const noexcept;
  void pretty_print(void) const noexcept;
  void write_to_file(const std::string &filename) const;

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
  std::ifstream _input_file;
  std::string _file_data;
  std::uint16_t longest_key_width; // for pretty_print function to adjust width.
  std::unique_ptr<parsed_data> _parsed_data;
  [[nodiscard("You have to check file extension!")]] static inline bool
  check_file_extension(const std::string_view &file);
};

inline void parser::print_raw_string() const noexcept {
  std::cout << _file_data << std::endl;
}

inline const parsed_data &parser::get_parsed_data(void) const noexcept {
  return *this->_parsed_data;
}

inline parsed_data &parser::get_parsed_data(void) noexcept {
  return *this->_parsed_data;
}

inline const std::string &parser::get_raw_string(void) const noexcept {
  return _file_data;
}

} // namespace ini
