// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#pragma once

#define FMT_HEADER_ONLY

///////////////////////////////////////////////
// need FMT lib: https://github.com/fmtlib/fmt

#include "../fmt/include/fmt/core.h"
///////////////////////////////////////////////
///////////////////////////////////////////////

#include "encrypted.h"
#include "source.h"

namespace encrypted {
class Gamemode {
public:
  Gamemode() {

    std::srand(std::time(0));
    encrypted::create_menu<encrypted::HelloMenu>(menu);
    attempm_id = encrypted::create_menu<encrypted::AttemptMenu>(menu);
    encr_id = encrypted::create_menu<encrypted::EncryptedMenu>(menu);
    input_id = encrypted::create_menu<encrypted::InputMenu>(menu);
  }

  inline void print_all_menu() const noexcept {

    for (const auto &e : menu) {
      fmt::print(fmt::fg(e->get_color()), e->to_str());
    }
  }

  // Print menu from id
  inline void print_menu(int id) const noexcept {

    fmt::print(fmt::fg(menu.at(id)->get_color()), menu.at(id)->to_str());
  }

  inline int get_attemp_id() const noexcept { return attempm_id; }
  inline int get_enct_id() const noexcept { return encr_id; }
  inline int get_input_id() const noexcept { return input_id; }

  void Run();

private:
  encrypted::vMenu menu;
  int attempm_id{};
  int encr_id{};
  int input_id{};
};
} // namespace encrypted
