// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

///////////////////////////////////////////////
// need FMT lib: https://github.com/fmtlib/fmt
#define FMT_HEADER_ONLY

#include "../fmt/include/fmt/core.h"
///////////////////////////////////////////////
///////////////////////////////////////////////

#include "encrypted.h"
#include "help.h"

#include "source.h"

int main() {

  try {
    std::srand(std::time(0));

    encrypted::enable_vir_term();

    encrypted::HelloMenu hello_menu;
    encrypted::EncryptedMenu encrypted_menu;

    fmt::print(fmt::fg(hello_menu.get_color()), hello_menu.to_str());
    fmt::print(fmt::fg(encrypted_menu.get_color()), encrypted_menu.to_str());

    return 0;
  }

  catch (std::runtime_error &e) {
    fmt::print(fmt::fg(fmt::color::red), e.what());
  }

  catch (...) {
    fmt::print(fmt::fg(fmt::color::red), "Error not found");
  }
}
