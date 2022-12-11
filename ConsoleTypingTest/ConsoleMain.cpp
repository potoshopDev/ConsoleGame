#include <iterator>
#include <cassert>
#include "ConsoleMain.h"
#include <iterator>

namespace console
{
	constexpr int count_words = 200;
};

bool console::ConsoleMain::initlize_console() noexcept
{

	if (handle == INVALID_HANDLE_VALUE)
	{
		assert(INVALID_HANDLE_VALUE);
		return false;
	}

	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
	if (!GetConsoleScreenBufferInfo(handle, &csbInfo))
	{
		assert(false);
		return false;
	}

	if (!SetConsoleWindowInfo(handle, true, &full_rect))
	{
		assert(false);
		return false;
	}

	if (!SetConsoleScreenBufferSize(handle, buffer_size))
	{
		assert(false);
		return false;
	}

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = m_FontSizeX;
	cfi.dwFontSize.Y = m_FontSizeX * 2;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	wcscpy_s(cfi.FaceName, L"Consolas");

	if (!SetCurrentConsoleFontEx(handle, false, &cfi))
	{
		assert(false);
		return false;
	}

	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = false;
	cci.dwSize = 1;
	if (!SetConsoleCursorInfo(handle, &cci))
	{
		assert(false);
		return false;
	}

	//disable resize console window 
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	/// ////////////////////////////////////

	if (!load_words()) return false;

	out_words();
	find_next_word();
	select_word();

	return true;
}

void console::ConsoleMain::out_words() noexcept
{
	std::wstringstream ws{ example_string };
	std::wstring tmp;
	auto i = 0;
	auto correct = 0;
	while (ws >> tmp)
	{
		if (i + correct + 2 + tmp.size() >= m_WinSizeX * ((correct / m_WinSizeX) + 1))
		{
			correct += m_WinSizeX;
			i = 0;
		}

		for (const auto& e : tmp)
		{
			m_CharBuffer[i++ + correct].Char.UnicodeChar = e;
		}

		m_CharBuffer[i++ + correct].Char.UnicodeChar = L' ';
	}
	buf_sz = correct + i;
}

void console::ConsoleMain::clear_console() noexcept
{
	std::for_each(m_CharBuffer.begin(),
		m_CharBuffer.end(),
		[](auto& x) mutable {x.Char.UnicodeChar = 0; x.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED; });

	std::for_each(m_CharBuffer.end() - m_WinSizeX * 2,
		m_CharBuffer.end() - m_WinSizeX,
		[](auto& x) mutable {x.Char.UnicodeChar = 0; x.Attributes = BACKGROUND_GREEN | BACKGROUND_RED; });

	myCursor.move_to_start();
	s_w = 0;
	e_w = 0;
	chk_char_pos = -1;

	is_timer_start = false;
	is_timeout = false;

	output_help();
}

bool console::ConsoleMain::reset_console() noexcept
{
	clear_console();
	if (!load_words()) return false;

	out_words();
	find_next_word();
	return true;
}

bool console::ConsoleMain::find_next_word()
{
	const auto end = buf_sz;
	if (s_w >= end || e_w >= end)
	{
		return false;
	}
	s_w = e_w == 0 ? s_w : ++e_w;
	for (; s_w < end; ++s_w)
	{
		if (m_CharBuffer[s_w].Char.UnicodeChar != L' ' && m_CharBuffer[s_w].Char.UnicodeChar != 0)
			break;
	}

	if (s_w < end)
		for (e_w = s_w; e_w < end; ++e_w)
		{
			if (m_CharBuffer[e_w].Char.UnicodeChar == L' ')
			{
				break;
			}
		}

	chk_char_pos = s_w - 1; //move check cursor;
	return true;
}

void console::ConsoleMain::select_word(WORD w_)
{
	for (auto x = s_w; x < e_w; ++x)
	{
		m_CharBuffer[x].Attributes = w_;
	}
}


void console::ConsoleMain::move_next_char(wchar_t w)
{
	if (++chk_char_pos > s_w + m_WinSizeX - 1) chk_char_pos = s_w + m_WinSizeX - 1;

	auto v = m_CharBuffer[chk_char_pos].Char.UnicodeChar;
	if (v == w && chk_char_pos < e_w)
	{
		m_CharBuffer[chk_char_pos].Attributes = BACKGROUND_GREEN;
	}
	else if (chk_char_pos < e_w)
	{
		m_CharBuffer[chk_char_pos].Attributes = BACKGROUND_RED;
	}

}

bool console::ConsoleMain::check_input()
{
	std::wstring word;
	for (auto x = s_w; x < e_w; ++x)
	{
		word += m_CharBuffer[x].Char.UnicodeChar;
	}

	if (word != input_word)
	{
		select_word(FOREGROUND_RED);
		return false;
	}
	return true;

}

void console::ConsoleMain::output_WPM()
{
	std::wstring out{ L"WPM: " };
	out += std::to_wstring(WPM);

	for (auto x = 0; x < out.size(); ++x)
	{
		auto xx = x + (m_WinSizeX * m_WinSizeY - (m_WinSizeX * 2));
		m_CharBuffer[xx].Char.UnicodeChar = out[x];
	}

}

void console::ConsoleMain::output_help()
{
	std::wstring out{ L"Reset: <Tab>; Exit: <Esc>" };

	for (auto x = 0; x < out.size(); ++x)
	{
		auto xx = x + (m_WinSizeX * m_WinSizeY - (m_WinSizeX * 3));
		m_CharBuffer[xx].Char.UnicodeChar = out[x];
		m_CharBuffer[xx].Attributes = FOREGROUND_GREEN | FOREGROUND_RED;
	}
}

void console::ConsoleMain::timer()
{
	end_clock = std::chrono::system_clock::now();
	std::chrono::duration<double> elapse_time = end_clock - start_clock;
	auto out_elepse = 60 - static_cast<int>(elapse_time.count());

	if (out_elepse <= 0)
	{
		out_elepse = 0;
		is_timeout = true;
		is_timer_start = false;
	}

	std::wstring out = (out_elepse < 10 ? L"0" : L"") + std::to_wstring(out_elepse) + L"s";

	for (auto x = 0; x < out.size(); ++x)
	{
		auto xx = x + (m_WinSizeX * m_WinSizeY - (m_WinSizeX + out.size()));
		m_CharBuffer[xx].Char.UnicodeChar = out[x];
	}
}

bool console::ConsoleMain::load_words() noexcept
{
	auto tmp_str = console::load_file::get_words(console::count_words);
	if (!check_and_copy_variant(tmp_str))
	{
		return false;
	}
	return true;
}

void console::ConsoleMain::Start() noexcept
{
	using namespace std::chrono_literals;
	myCursor.SetCursorBlinkDuration(300ms);

	while (!is_exit)
	{
		auto [pressed, key_num] = console_input.GetPressedKey();

		const auto cursor_pos = myCursor.GetCurrentCursorPosition();
		if (pressed)
		{
			switch (key_num)
			{
			case VK_ESCAPE:
			{
				is_exit = true;
				break;
			}
			case VK_BACK:
			{
				if (!is_timeout && is_timer_start)
				{
					console_input.IsCtrlPressed() ? ClearInputLilne() : RemoveLastChar(cursor_pos);
				}
				break;
			}
			case VK_TAB:
			{
				input_word.clear();
				reset_console();
				select_word();
				WPM = 0;

				break;
			}
			case VK_SPACE:
			{
				if (!is_timeout && is_timer_start)
				{
					if (check_input())
					{
						++WPM;
					}

					find_next_word();
					select_word();
					ClearInputLilne();
				}
				break;
			}

			default:

				if (!is_timer_start)
				{
					start_clock = std::chrono::system_clock::now();
				}

				if (!is_timeout)
				{
					is_timer_start = true;
					input_word += key_num;
					AddNewChar(cursor_pos, key_num);
					move_next_char(key_num);
				}
				break;
			}
		}

		output_WPM();
		if (is_timer_start)
		{
			timer();
		}
		const auto update_cursor_pos = myCursor.GetCurrentCursorPosition();
		m_CharBuffer[update_cursor_pos].Char.UnicodeChar = myCursor.GetCursorChar();

		Draw();
	}
}

void console::ConsoleMain::AddNewChar(const short& cursor_pos, wchar_t key_num) noexcept
{
	//add one char
	m_CharBuffer[cursor_pos].Char.UnicodeChar = key_num; //TODO: new char

	++myCursor;

}

bool console::ConsoleMain::check_and_copy_variant(result_load r)
{
	auto result = false;
	std::visit(overloaded
		{
			[&result](int& i) { result = false; },
			[this, &result](std::wstring& s) {example_string = s; result = true; }
		}
	, r);
	return result;
}

void console::ConsoleMain::RemoveLastChar(const short& cursor_pos) noexcept
{
	//clear one char
	m_CharBuffer[cursor_pos].Char.UnicodeChar = 0;
	--myCursor;

	if (chk_char_pos >= s_w && chk_char_pos < e_w)
	{
		m_CharBuffer[chk_char_pos].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_INTENSITY;
	}
	else if (chk_char_pos >= s_w)
	{
		m_CharBuffer[chk_char_pos].Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

	}

	if (--chk_char_pos < s_w) chk_char_pos = s_w - 1;

	if (!input_word.empty()) input_word.pop_back();
}

void console::ConsoleMain::ClearInputLilne() noexcept
{
	//clear line
	std::for_each(m_CharBuffer.begin() + (m_WinSizeX * (m_WinSizeY - 1)),
		m_CharBuffer.end(),
		[](auto& x) mutable {x.Char.UnicodeChar = 0; });

	myCursor.move_to_start();

	//clear select
	select_word();
	chk_char_pos = s_w - 1;

	input_word.clear();
}

void console::ConsoleMain::Draw() noexcept
{
	WriteConsoleOutput(handle, &(m_CharBuffer.front()), buffer_size, { 0, 0 }, &full_rect);
}
