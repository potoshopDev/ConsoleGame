// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#include "source.h"
#define FMT_HEADER_ONLY

#include "../fmt/include/fmt/color.h"
#include "../fmt/include/fmt/core.h"

#include "encrypted.h"
#include "help.h"

// random
#include <cstdlib>
#include <ctime>

int main() {

  try {
    srand(time(0));
    encrypted::enable_vir_term();

    encrypted::Wordbook wb(encrypted::load_wb(encrypted::file_name));
    // Test class encrypted_line
    for (auto i = 0; i < 20; ++i) {
      auto ind1 = rand() % (wb.get_size());
      auto ind2 = rand() % (wb.get_size());

      encrypted::encrypted_line test((rand() % 100 < 10) ? wb.get_word(ind1)
                                                         : "");

      encrypted::encrypted_line test2((rand() % 100 < 10) ? wb.get_word(ind2)
                                                          : "");

      fmt::print(fmt::fg(fmt::color::green_yellow), test.get_line());
      fmt::print(fmt::fg(fmt::color::green_yellow), test2.get_line());
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
