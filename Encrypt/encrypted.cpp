// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#include "encrypted.h"
#include <algorithm>
#include <fstream>
#include <sstream>

#include "help.h"

namespace encrypted {

wb load_wb(const std::string &file_name) {

  std::ifstream ifile(file_name.c_str());
  if (!ifile.is_open()) {
    error("File not found");
  }

  std::istream_iterator<std::string> it_str{ifile};
  std::istream_iterator<std::string> it_end;
  wb tmp_vec;

  std::copy(it_str, it_end, std::back_inserter(tmp_vec));

  return tmp_vec;
}

std::string EncryptedLine::gnr_direction_line() {

  auto number{num_str_range + rand() % num_end_range};

  std::stringstream tmp_ss;
  tmp_ss << std::hex << std::showbase << number;

  return tmp_ss.str();
}

std::string EncryptedLine::gnr_body_line() {

  auto pos{rand() % pos_end_range}; // Random position for writing a word

  std::string str_tmp;

  for (auto i{0}; i < pos; ++i) {

    str_tmp += get_symbol();
  }

  for (const auto e : m_word) {
    if (str_tmp.size() == pos_end_word)
      str_tmp += " ";

    str_tmp += e;
  }

  for (auto i{str_tmp.size()}; i < line_size; ++i) {
    if (i == pos_end_word) {
      str_tmp += " ";
    } else {
      str_tmp += get_symbol();
    }
  }

  return str_tmp;
}

void EncryptedLine::gnr_line() {

  auto dir_str{gnr_direction_line()}; // hex format line number
  auto body_str{gnr_body_line()};
  const auto space{std::string(" ")};

  m_line = dir_str + space + body_str + space;
  toupper_string(m_line);
}

std::string EncryptedLine::get_line() {

  if (m_line.empty()) {
    gnr_line();
  }

  return m_line;
}

// ASCII 33 to 64 random symbol
inline char get_symbol() { return smb_str_range + rand() % smb_end_range; }

void toupper_string(std::string &line) {
  std::for_each(line.begin(), line.end(), [](auto &e) { e = std::toupper(e); });
}

EncryptedMenu::EncryptedMenu() {

  // initilized vector
  for (auto i{0}; i < max_lines; ++i) {

    m_data_lines.push_back(std::make_unique<EncryptedLine>(EncryptedLine("")));
  }
  ////////////////////

  // initilized first word == encrypted word
  Wordbook wb(load_wb(file_name));

  const int str_wrd_ind = rand() % (wb.get_size() - max_word);
  const auto str_lns = rand() % (max_lines);

  std::vector<int> tmp_lns{str_lns}; // saving of the occupied line num
  encrypted_wrd = wb.get_word(str_wrd_ind);
  toupper_string(encrypted_wrd);

  pchange(str_lns, str_wrd_ind, wb);
  ////////////////////

  // initilized the others word
  for (auto i{0}; i < max_word; ++i) {
    int new_wrd_pos{0};

    // find next fake line
    for (auto bid_cor{true}; bid_cor;) {

      bid_cor = false;
      new_wrd_pos = rand() % max_lines;

      std::for_each(tmp_lns.begin(), tmp_lns.end(),
                    [&bid_cor, new_wrd_pos](const auto &e) {
                      bid_cor = bid_cor ? true : e == new_wrd_pos;
                    });
    }

    tmp_lns.push_back(new_wrd_pos);
    const int new_wrd_ind = str_wrd_ind + rand() % max_word;

    pchange(new_wrd_pos, new_wrd_ind, wb);
  }
}

void EncryptedMenu::pchange(int pos, int id, const Wordbook &wb) {
  // m_data_lines.at(pos).reset();
  m_data_lines.at(pos) =
      std::make_unique<EncryptedLine>(EncryptedLine(wb.get_word(id)));
}
} // namespace encrypted
