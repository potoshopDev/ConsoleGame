#pragma once

#include <chrono>

namespace console
{
	class ConsoleCursor
	{
	public:
		explicit ConsoleCursor(short start_, short end_) :
			m_start{ start_ },
			m_end{ end_ },
			blink_duration{ 100 },
			visible_{ true },
			m_current{ m_start },
			cursor_blink_flag{ true }
		{
			t0_ = std::chrono::high_resolution_clock::now();

		}

		inline void SetCursorVisible(bool is_visible)
		{
			visible_ = is_visible;
		}

		ConsoleCursor& operator++();
		ConsoleCursor& operator--();
		ConsoleCursor operator++(int);
		ConsoleCursor operator--(int);

		[[nodiscard]] inline short GetCurrentCursorPosition() const noexcept
		{
			return m_current;
		}
		[[nodiscard]] inline short GetNextCurrentCursorPosition() noexcept
		{
			++(*this);
			return m_current;
		}
		[[nodiscard]] inline short GetPreCurrentCursorPosition() noexcept
		{
			--(*this);
			return m_current;
		}
		[[nodiscard]] inline wchar_t GetCursorChar() noexcept
		{
			if (visible_)
			{
				auto t1 = std::chrono::high_resolution_clock::now();
				auto t_result = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0_);
				if (t_result > blink_duration)
				{
					cursor_blink_flag = !cursor_blink_flag;
					t0_ = t1;
				}
				return cursor_blink_flag ? cursor_char : L' ';
			}

			return L' ';
		}

		inline void SetCursorBlinkDuration(std::chrono::milliseconds bd)
		{
			blink_duration = bd;
		}
		inline void move_to_start()
		{
			m_current = m_start;
		}

	private:
		[[nodiscard]] bool is_last() noexcept
		{
			return m_current == m_end;
		}
		[[nodiscard]] bool is_next_last() noexcept
		{
			return m_current + 1 > m_end;
		}
		[[nodiscard]] bool is_next_prefirst() noexcept
		{
			return m_current - 1 < m_start;
		}
	private:
		const short m_start;
		const short m_end;
		short m_current;

		const wchar_t cursor_char = 0x2588;

		std::chrono::milliseconds blink_duration;

		bool visible_;
		bool cursor_blink_flag;

		std::chrono::steady_clock::time_point t0_;


	};
}
