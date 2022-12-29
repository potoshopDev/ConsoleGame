// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

///////////////////////////////////////////////
// need FMT lib: https://github.com/fmtlib/fmt
#include <limits.h>
#define FMT_HEADER_ONLY

#include "../fmt/include/fmt/core.h"
///////////////////////////////////////////////
///////////////////////////////////////////////

#include "encrypted.h"
#include "help.h"

#include "source.h"

#include <iostream>
#include <limits>

int main() {

  try {
    std::srand(std::time(0));

    encrypted::enable_vir_term();

    encrypted::vMenu menu;
    encrypted::create_menu<encrypted::HelloMenu>(menu);
    auto attempm_id = encrypted::create_menu<encrypted::AttemptMenu>(menu);
    auto encr_id = encrypted::create_menu<encrypted::EncryptedMenu>(menu);

    for (const auto &e : menu) {

      fmt::print(fmt::fg(e->get_color()), e->to_str());
    }

    encrypted::create_menu<encrypted::InputMenu>(menu);

    // print input line
    for (auto game_run{true}; game_run;) {
      fmt::print(fmt::fg(menu.back()->get_color()), menu.back()->to_str());

      std::string input_word;
      std::cin >> input_word;

      std::cin.clear();
      std::cin.ignore(INT_MAX, '\n'); // check only the first input word

      auto count_eql{
          encrypted::check_password(menu.at(encr_id).get(), input_word)};

      game_run = !(count_eql == encrypted::max_char);
      auto curr_attemp = encrypted::update_attemp(menu.at(attempm_id).get());

      const std::string check_message{
          !game_run ? "Password is correct!" : "Entry denied. {}/{} correct"};

      // check count attempts
      game_run = curr_attemp < 1 && game_run ? !game_run : game_run;

      fmt::print(fmt::fg(fmt::color::green), check_message.c_str(), count_eql,
                 encrypted::max_char);

      // print game status. continue or gameover
      if (game_run) {
        fmt::print(fmt::fg(menu.at(attempm_id)->get_color()),
                   menu.at(attempm_id)->to_str());
      } else if (curr_attemp < 1) {

        fmt::print(fmt::fg(fmt::color::orange_red),
                   "\nIncorrect PASSWORD. ACCESS DENIED");
      }
    }

    return 0;
  }

  catch (std::runtime_error &e) {
    fmt::print(fmt::fg(fmt::color::red), e.what());
  }

  catch (...) {
    fmt::print(fmt::fg(fmt::color::red), "Error not found");
  }
}
