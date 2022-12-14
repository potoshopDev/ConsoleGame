// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
//
// Fanart based on the mini-game "hacker" fallout 4

#include "encrypted.h"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "help.h"
#include "source.h"

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

  auto number{num_str_range + std::rand() % num_end_range};

  std::stringstream tmp_ss;
  tmp_ss << std::hex << std::showbase << number;

  return tmp_ss.str();
}

std::string EncryptedLine::gnr_body_line() {

  auto pos{rand() % pos_end_range}; // std::random position for writing a word

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

// ASCII 33 to 64 std::random symbol
inline char get_symbol() { return smb_str_range + std::rand() % smb_end_range; }

void toupper_string(std::string &line) {
  std::for_each(line.begin(), line.end(), [](auto &e) { e = std::toupper(e); });
}

EncryptedMenu::EncryptedMenu() {
  m_color = fmt::color::green_yellow;
  // initilized vector
  for (auto i{0}; i < max_lines; ++i) {

    m_data_lines.emplace_back(
        std::make_unique<EncryptedLine>(EncryptedLine("")));
  }
  ////////////////////

  // initilized first word == encrypted word
  Wordbook wb(load_wb(file_name));

  const int str_wrd_ind = std::rand() % (wb.get_size() - max_word);
  const auto str_lns = std::rand() % (max_lines);

  std::vector<int> tmp_lns{str_lns}; // saving of the occupied line num

  encrypted_wrd = wb.get_word(str_wrd_ind);
  toupper_string(encrypted_wrd);

  pchange(str_lns, str_wrd_ind, wb);
  ////////////////////

  gnr_fakelines(tmp_lns, wb, str_wrd_ind);
}

void EncryptedMenu::pchange(int pos, int id, const Wordbook &wb) {
  // m_data_lines.at(pos).reset();
  m_data_lines.at(pos) =
      std::make_unique<EncryptedLine>(EncryptedLine(wb.get_word(id)));
}

void EncryptedMenu::gnr_fakelines(std::vector<int> &v, const Wordbook &wb,
                                  const int str_id) {

  for (auto i{0}; i < max_word; ++i) {

    auto rnd_line = []() { return std::rand() % max_lines; };
    int new_wrd_pos{rnd_line()};

    // find next fake line
    for (; std::find(v.begin(), v.end(), new_wrd_pos) != v.end();) {

      new_wrd_pos = rnd_line();
    }

    v.push_back(new_wrd_pos);

    const int new_wrd_ind =
        (str_id - max_word / 2) + std::rand() % (max_word / 2);

    pchange(new_wrd_pos, new_wrd_ind, wb);
  }
}

const std::string EncryptedMenu::to_str() {
  std::string tmp_str{};
  std::for_each(m_data_lines.begin(), m_data_lines.end(),
                [&tmp_str, i = 0](const auto &e) mutable {
                  if (i == 2) {
                    tmp_str += "\n";
                    i = 0;
                  }
                  tmp_str += e->get_line();
                  ++i;
                });
  return tmp_str;
}
[[nodiscard]] int EncryptedMenu::check(const std::string &cw) const {
  auto tmp_str{cw};
  toupper_string(tmp_str);

  auto tmp_eql{0};
  std::equal(encrypted_wrd.begin(), encrypted_wrd.end(), tmp_str.begin(),
             [&tmp_eql](const auto &e1, const auto &e2) {
               if (e1 == e2)
                 ++tmp_eql;
               return true;
             });

  return tmp_eql;
}

int check_password(const Menu *encr_menu, const std::string &cw) {

  auto em = static_cast<const EncryptedMenu *>(encr_menu);
  if (!em)
    error("Menu is not correct");

  return em->check(cw);
}
[[nodiscard]] int update_attemp(Menu *attmp_menu) {

  auto am = static_cast<AttemptMenu *>(attmp_menu);
  if (!am)
    error("Menu is not correct");

  return am->get_m_attempt();
}
} // namespace encrypted
