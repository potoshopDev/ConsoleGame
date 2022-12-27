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

#include "../fmt/include/fmt/color.h"

namespace encrypted {

using wb = std::vector<std::string>;

inline char get_symbol();
void toupper_string(std::string &line);
wb load_wb(const std::string &file_name);

class Menu {
public:
  virtual ~Menu(){};
  virtual const std::string to_str() = 0;
  virtual const fmt::color get_color() noexcept { return m_color; };
  operator const std::string() { return to_str(); }

protected:
  fmt::color m_color{fmt::color::white};
};

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

class EncryptedMenu : public Menu {
public:
  using EM = std::unique_ptr<encrypted::EncryptedLine>;
  EncryptedMenu();

  const std::string to_str() override;

  [[nodiscard]] inline const std::string get() const noexcept {
    return encrypted_wrd;
  }

private:
  void pchange(int pos, int id, const Wordbook &wb);
  void gnr_fakeline(std::vector<int> &v, const Wordbook &wb, const int str_id);

private:
  std::vector<EM> m_data_lines{};
  std::string encrypted_wrd{};
};

class HelloMenu : public Menu {
public:
  HelloMenu()
      : m_menu_text("Hello! Welcome to encryped!\n"
                    "Try to guess the password\n\n") {
    m_color = fmt::color::green;
  };

  const std::string to_str() override { return m_menu_text; }

private:
  std::string m_menu_text;
};

} // namespace encrypted
