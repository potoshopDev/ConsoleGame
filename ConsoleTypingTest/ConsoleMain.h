#pragma once

#define _WIN32_WINNT 0x0500

#include <Windows.h>
#include <vector>
#include "ConsoleCursor.h"
#include "ConsoleInput.h"
#include "load_file.h"

namespace console
{
	class ConsoleMain
	{
	public:
		explicit ConsoleMain(short sizeX = 100i16, short sizeY = 20i16, short font_size = 12i16) :
			m_WinSizeX{ sizeX },
			m_WinSizeY{ sizeY },
			m_FontSizeX{ font_size },
			buffer_size{ m_WinSizeX, m_WinSizeY },
			full_rect{ 0,0, m_WinSizeX - 1, m_WinSizeY - 1 },
			myCursor{ m_WinSizeX * (m_WinSizeY - 1), m_WinSizeX * m_WinSizeY - 1 }

		{
			handle = GetStdHandle(STD_OUTPUT_HANDLE);

			CHAR_INFO ci;
			ci.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
			ci.Char.UnicodeChar = 0;
			m_CharBuffer.assign(m_WinSizeX * m_WinSizeY, ci);
			clear_console();


		};
		[[nodiscard]] bool initlize_console() noexcept;
		void Start() noexcept;




	private:

		void Draw() noexcept;
		void ClearInputLilne() noexcept;
		void RemoveLastChar(const short& cursor_pos) noexcept;
		void AddNewChar(const short& cursor_pos, wchar_t key_num) noexcept;


		template <typename ... T>
		struct overloaded : T ...
		{
			using T::operator() ...;
		};
		//template <typename ... T>
		//overloaded(T...) -> overloaded<T...>;


		using result_load = std::variant<int, std::wstring>;
		bool check_and_copy_variant(result_load r);

		[[nodiscard]] bool load_words() noexcept;
		void out_words() noexcept;

		void clear_console() noexcept;
		bool reset_console() noexcept;

		bool find_next_word();
		void select_word(WORD w_ =  FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | BACKGROUND_INTENSITY);
		void move_next_char(wchar_t w);
		[[nodiscard]] bool check_input();
		void output_WPM();
		void output_help();

		void timer();

	private:

		const short m_WinSizeX;
		const short m_WinSizeY;
		const short m_FontSizeX;
		const COORD buffer_size;

		HANDLE handle;
		std::vector<CHAR_INFO> m_CharBuffer;
		SMALL_RECT full_rect;

		ConsoleCursor myCursor;
		ConsoleInput console_input;
		std::wstring example_string;
		bool is_exit = false;

		int s_w = 0;
		int e_w = 0;
		int buf_sz = 0;

		int chk_char_pos = -1;
		std::wstring input_word;

		unsigned WPM = 0;

		std::chrono::time_point<std::chrono::system_clock> start_clock, end_clock;
		bool is_timer_start = false;
		bool is_timeout = false;

	};

}
 