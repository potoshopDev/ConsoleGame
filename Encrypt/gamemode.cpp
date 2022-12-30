// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#include "gamemode.h"
#include <iostream>

namespace encrypted {

void Gamemode::Run() {

  for (auto game_run{true}; game_run;) {

    std::string input_word;
    std::cin >> input_word;

    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n'); // check only the first input word

    auto count_eql{
        encrypted::check_password(menu.at(encr_id).get(), input_word)};

    game_run = !(count_eql == encrypted::max_char);

    const std::string check_message{!game_run ? "Password is correct!"
                                              : "Entry denied. {}/{} correct"};

    fmt::print(fmt::fg(fmt::color::green), check_message.c_str(), count_eql,
               encrypted::max_char);

    auto curr_attemp = encrypted::update_attemp(menu.at(attempm_id).get());
    // check count attempts
    game_run = curr_attemp < 1 && game_run ? !game_run : game_run;

    // print game status. continue or gameover
    if (game_run) {
      // print attemp menu
      print_menu(get_attemp_id());
      // print input line
      print_menu(get_input_id());

    } else if (curr_attemp < 1 && count_eql != encrypted::max_char) {

      fmt::print(fmt::fg(fmt::color::orange_red),
                 "\nIncorrect PASSWORD. ACCESS DENIED");
    }
  }
}

} // namespace encrypted
