// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#include "encrypted.h"
#include <algorithm>
#include <random>
#include <sstream>
#include <string>

#include "source.h"

namespace encrypted {

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

    str_tmp += GetSymbol();
  }

  for (const auto e : word) {
    if (str_tmp.size() == pos_end_word)
      str_tmp += " ";

    str_tmp += e;
  }

  if (str_tmp.size() == pos_end_word)
    str_tmp += " ";

  for (auto i = str_tmp.size(); i < line_size; ++i) {
    str_tmp += GetSymbol();
  }

  return str_tmp;
}

void encrypted_line::generate_line() {

  auto dir_str = gnr_direction_line(); // hex format line number
  auto body_str = gnr_body_line();

  line = {dir_str + std::string(" ") + body_str + std::string(" ")};
  toupper_line(line);
}

std::string encrypted_line::GetLine() {

  if (line.empty()) {
    generate_line();
  }

  return line;
}

// ASCII 33 to 64 random symbol
inline char GetSymbol() { return smb_str_range + rand() % smb_end_range; }

void toupper_line(std::string &line) {
  std::for_each(line.begin(), line.end(), [](auto &e) { e = std::toupper(e); });
}
} // namespace encrypted
