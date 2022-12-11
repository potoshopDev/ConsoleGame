#include "ConsoleInput.h"

std::pair<bool, wchar_t> console::ConsoleInput::GetPressedKey() noexcept
{
	DWORD event = 0;
	GetNumberOfConsoleInputEvents(hi, &event);
	INPUT_RECORD inBuf[32];
	ReadConsoleInput(hi, inBuf, event, &event);


	for (DWORD i = 0; i < event; ++i)
	{
		switch (inBuf[i].EventType)
		{
		case FOCUS_EVENT:
			focus = inBuf[i].Event.FocusEvent.bSetFocus;
			break;
		default:
			break;
		}
	}
	if (focus)
	{
		bool shift_presed = false;
		if (GetAsyncKeyState(VK_LSHIFT) & 0x8000 || GetAsyncKeyState(VK_RSHIFT) & 0x8000)
		{
			shift_presed = true;
		}
		if (GetAsyncKeyState(VK_CAPITAL) & 0x01)
		{
			caps_presed = !caps_presed;
		}


		for (auto i = 0; i < console::countkey; ++i)
		{
			short key = 0;
			if ((key = GetAsyncKeyState(i)) & 0x01)
			{
				if ((i >= 0x41 && i <= 0x5A) || (i >=0x30 && i<=0x39) ) //0x41 - A && 0x5A - Z && 0x30 - 1 && 0x39 - 9
				{
					wchar_t w = i;

					if (!((!shift_presed && caps_presed) || (shift_presed && !caps_presed)))
						w = tolower(w);

					return std::pair<bool, wchar_t>(true, w);
				}
				if (i == VK_BACK || i == VK_SPACE || i==VK_TAB || i==VK_ESCAPE)
				{
					return std::pair<bool, wchar_t>(true, i);
				}
				if (i == VK_OEM_MINUS)
				{
					return std::pair<bool, wchar_t>(true, L'-');
				}
				if (i == 188)
				{
					return std::pair<bool, wchar_t>(true, L',');
				}
				if (i == 190)
				{
					return std::pair<bool, wchar_t>(true, L'.');
				}
			}


		}
	}
	return std::pair<bool, wchar_t>(false, 0);
}

bool console::ConsoleInput::IsCtrlPressed() noexcept
{
	return (GetAsyncKeyState(VK_CONTROL) & 0x8000);
}
