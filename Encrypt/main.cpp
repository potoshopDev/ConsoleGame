// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#define FMT_HEADER_ONLY

#include "../fmt/include/fmt/color.h"
#include "../fmt/include/fmt/core.h"

#include "encrypted.h"
#include "help.h"

#include <ctime>

int main() {

  try {
    srand(time(0));
    encrypted::enable_vir_term();

    // Test class encrypted_line
    for (auto i = 0; i < 20; ++i) {

      encrypted::encrypted_line test("cout");
      encrypted::encrypted_line test2("game");

      fmt::print(fmt::fg(fmt::color::green_yellow), test.GetLine());
      fmt::print(fmt::fg(fmt::color::green_yellow), test2.GetLine());
      fmt::print("\n");
    }
    /////////////////////////////

    return 0;
  }

  catch (std::runtime_error &e) {
    fmt::print(fmt::fg(fmt::color::red), e.what());
  }

  catch (...) {
    fmt::print(fmt::fg(fmt::color::red), "Error not found");
  }
}
