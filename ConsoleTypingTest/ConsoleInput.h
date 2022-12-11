#pragma once
#include <Windows.h>
#include <utility>

namespace console
{
	constexpr short countkey = 256;
	
	class ConsoleInput
	{
	public:
		ConsoleInput() {};

		std::pair<bool, wchar_t> GetPressedKey() noexcept;
		[[nodiscard]] bool IsCtrlPressed() noexcept;

	private:
	bool caps_presed = false;
	bool focus = true;

	HANDLE hi = GetStdHandle(STD_INPUT_HANDLE);
	};
}
