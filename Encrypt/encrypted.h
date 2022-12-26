// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4
#pragma once

#include <string>
#include <vector>

namespace encrypted {

using wb = std::vector<std::string>;

inline char get_symbol();
void toupper_string(std::string &line);
wb load_wb(const std::string &file_name);

class Wordbook {
public:
  Wordbook(wb words) : wordbook__{words} {};

  [[nodiscard]] inline std::string get_word(int index) const {
    return wordbook__.at(index);
  }
  [[nodiscard]] inline size_t get_size() const { return wordbook__.size(); }

private:
  wb wordbook__;
};

class encrypted_line {
public:
  encrypted_line(const std::string word__) : word{word__} {}
  std::string get_line();

private:
  void gnr_line();
  std::string gnr_direction_line();
  std::string gnr_body_line();

private:
  std::string line;
  std::string word;
};

} // namespace encrypted
