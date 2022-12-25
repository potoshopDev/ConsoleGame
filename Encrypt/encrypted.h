// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#include <cstdlib>
#include <string>

namespace encrypted {

inline char GetSymbol();
void toupper_line(std::string &line);

class encrypted_line {
public:
  encrypted_line(const std::string word__) : word{word__} {}
  std::string GetLine();

private:
  void generate_line();
  std::string gnr_direction_line();
  std::string gnr_body_line();

private:
  std::string line;
  std::string word;
};

} // namespace encrypted
