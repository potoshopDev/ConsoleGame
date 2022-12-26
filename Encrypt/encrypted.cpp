// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#include "encrypted.h"
#include <algorithm>
#include <fstream>
#include <sstream>

#include "help.h"
#include "source.h"

namespace encrypted {

wb load_wb(const std::string &file_name) {

  std::ifstream ifile(file_name.c_str());
  if (!ifile.is_open()) {
    error("File not found");
  }

  std::istream_iterator<std::string> it_str{ifile};
  std::istream_iterator<std::string> it_end;
  wb tmp_vec;

  std::copy(it_str, it_end, std::back_inserter(tmp_vec));

  return std::move(tmp_vec);
}

std::string encrypted_line::gnr_direction_line() {

  auto number = num_str_range + rand() % num_end_range;

  std::stringstream tmp_ss;
  tmp_ss << std::hex << std::showbase << number;

  return std::string(tmp_ss.str());
}

std::string encrypted_line::gnr_body_line() {

  auto pos = rand() % pos_end_range; // Random position for writing a word

  std::string str_tmp;

  for (auto i = 0; i < pos; ++i) {

    str_tmp += get_symbol();
  }

  for (const auto e : word) {
    if (str_tmp.size() == pos_end_word)
      str_tmp += " ";

    str_tmp += e;
  }

  for (auto i = str_tmp.size(); i < line_size; ++i) {
    if (str_tmp.size() == pos_end_word) {
      str_tmp += " ";
    } else {
      str_tmp += get_symbol();
    }
  }

  return str_tmp;
}

void encrypted_line::gnr_line() {

  auto dir_str = gnr_direction_line(); // hex format line number
  auto body_str = gnr_body_line();

  line = {dir_str + std::string(" ") + body_str + std::string(" ")};
  toupper_string(line);
}

std::string encrypted_line::get_line() {

  if (line.empty()) {
    gnr_line();
  }

  return line;
}

// ASCII 33 to 64 random symbol
inline char get_symbol() { return smb_str_range + rand() % smb_end_range; }

void toupper_string(std::string &line) {
  std::for_each(line.begin(), line.end(), [](auto &e) { e = std::toupper(e); });
}
} // namespace encrypted
