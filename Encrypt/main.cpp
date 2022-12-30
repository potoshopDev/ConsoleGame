// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#include "gamemode.h"
#include "help.h"

int main() {

  try {
    encrypted::enable_vir_term();

    encrypted::Gamemode game_mode;
    game_mode.print_all_menu();
    game_mode.Run();

    return 0;
  }

  catch (std::runtime_error &e) {
    fmt::print(fmt::fg(fmt::color::red), e.what());
  }

  catch (...) {
    fmt::print(fmt::fg(fmt::color::red), "Error not found");
  }
}
