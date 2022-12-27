// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4
#pragma once

#include "source.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace encrypted {

using wb = std::vector<std::string>;

inline char get_symbol();
void toupper_string(std::string &line);
wb load_wb(const std::string &file_name);

class Wordbook {
public:
  Wordbook(wb words) : m_wordbook{words} {};

  [[nodiscard]] inline std::string get_word(int index) const {
    return m_wordbook.at(index);
  }
  [[nodiscard]] inline size_t get_size() const { return m_wordbook.size(); }

private:
  wb m_wordbook;
};

class EncryptedLine {
public:
  EncryptedLine(const std::string word__) : m_word{word__} {}
  std::string get_line();

private:
  void gnr_line();
  std::string gnr_direction_line();
  std::string gnr_body_line();

private:
  std::string m_line;
  std::string m_word;
};

class EncryptedMenu {
public:
  using EM = std::unique_ptr<encrypted::EncryptedLine>;
  EncryptedMenu();

  const std::string to_str() {
    std::string tmp_str{};
    std::for_each(m_data_lines.begin(), m_data_lines.end(),
                  [&tmp_str, i = 0](const auto &e) mutable {
                    if (i == 2) {
                      tmp_str += "\n";
                      i = 0;
                    }
                    tmp_str += e->get_line();
                    ++i;
                  });
    return tmp_str;
  }

  operator const std::string() { return to_str(); }

  [[nodiscard]] inline const std::string get() const noexcept {
    return encrypted_wrd;
  }

private:
  void pchange(int pos, int id, const Wordbook &wb);

private:
  std::vector<EM> m_data_lines{};
  std::string encrypted_wrd{};
};

} // namespace encrypted
