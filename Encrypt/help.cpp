// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#include "help.h"

#define WIN32_LEAN_AND_MEAN
#include <stdexcept>
#include <windows.h>

namespace encrypted {
using namespace std::string_literals;
void enable_vir_term() {
  auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
  if (handle == INVALID_HANDLE_VALUE) {
    error("Console handle error"s);
  }

  DWORD mode = 0;
  if (!GetConsoleMode(handle, &mode)) {
    error("Console mode error"s);
  }

  mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  if (!SetConsoleMode(handle, mode)) {
    error("Set console mode error"s);
  }
  // PCONSOLE_SCREEN_BUFFER_INFOEX buffer_info;
  // PCONSOLE_SCREEN_BUFFER_INFO buffer_info2;
  // if (!GetConsoleScreenBufferInfo(handle, buffer_info2)) {
  //   fmt::print(fmt::fg(fmt::color::red), "Get Console buffer error\n");
  // }
  //
  // COORD wnd_size{100, 25};
  // SMALL_RECT wnd_rct{0, 0, 99, 24};
  // buffer_info->dwMaximumWindowSize = wnd_size;
  // buffer_info->dwMaximumWindowSize = wnd_size;
  // buffer_info->srWindow = wnd_rct;
  //
  // buffer_info2->dwMaximumWindowSize = wnd_size;
  // buffer_info2->dwMaximumWindowSize = wnd_size;
  // buffer_info2->srWindow = wnd_rct;
  //
  // if (!SetConsoleScreenBufferSize(handle, wnd_size)) {
  //   fmt::print(fmt::fg(fmt::color::red), "Console buffer error\n");
  // }
}

void error(std::string error_message) {

  throw std::runtime_error("Error: "s + error_message + "\n"s);
}
} // namespace encrypted
