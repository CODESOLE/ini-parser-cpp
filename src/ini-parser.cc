// Copyright (c) 2022 CODESOLE
// ini-parser.cc: ini-parser definitions
// SPDX-License-Identifier: MIT

#include "ini-parser.hh"
#include <algorithm>
#include <filesystem>
#include <regex>

using namespace ini;

inline bool parser::check_file_extension(const std::string_view &file) {
  return file.ends_with(".ini");
}

parser::parser(const std::string_view &in_file)
    : _file_data(std::filesystem::file_size(in_file), '0') {
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

static void get_section(std::string line, parsed_data &pd) {
  if (std::smatch sm;
      regex_search(line, sm, std::regex(R"(\[[^\]]*\])")) && sm.size() == 1) {
    is_root = false;
    for (auto elm : sm) {
      curr_section = std::string(elm.str().begin() + 1, elm.str().end() - 1);
      pd[curr_section] = {{}, {}};
    }
  }
}

static void get_property(std::string line, parsed_data &pd) {
  if (std::smatch sm;
      regex_search(line, sm, std::regex(R"(^.*=.*$)")) && sm.size() == 1) {
    std::string spair[2];
    for (auto &elm : sm) {
      std::string::size_type pos = 0;
      std::string token = elm.str();
      while ((pos = token.find("=")) != std::string::npos) {
        trim_line(token);
        spair[0] = token.substr(0, pos);
        token.erase(0, pos + 1);
      }
      trim_line(token);
      spair[1] = token;
      if (is_root) {
        pd["root"].insert(std::make_pair(spair[0], spair[1]));
      } else {
        pd[curr_section].insert(std::make_pair(spair[0], spair[1]));
      }
    }
  }
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
    get_property(line, parsed);
  }
  this->_parsed_data = std::make_unique<parsed_data>(parsed);
}
