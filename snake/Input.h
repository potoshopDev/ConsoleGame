#pragma once
#include <Windows.h>

namespace emodule
{
	constexpr short countkey = 256;

	class IInput
	{
	public:
		virtual ~IInput() = default;

		virtual void GetFocus() noexcept = 0;
		virtual void GetPressedKey() noexcept = 0;
	};


	class ConsoleInput : public IInput
	{
	public:

		void GetFocus() noexcept override;
		void GetPressedKey() noexcept override;

	private:

		bool focus = true;
		HANDLE hi = GetStdHandle(STD_INPUT_HANDLE);
	};
}

