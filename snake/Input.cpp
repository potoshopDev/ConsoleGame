#include "Input.h"
#include "include/Core.h"

void emodule::ConsoleInput::GetFocus() noexcept
{
	DWORD event = 0;
	GetNumberOfConsoleInputEvents(hi, &event);
	std::vector<INPUT_RECORD> inBuf(32);
	ReadConsoleInput(hi, &inBuf.front(), event, &event);

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
}

void emodule::ConsoleInput::GetPressedKey() noexcept
{
	if (!focus) return;

	for (auto i = 0; i < emodule::countkey; ++i)
	{
		if (GetAsyncKeyState(i) & 0x01)
		{
			if (i == VK_LEFT || i == VK_UP || i == VK_RIGHT || i == VK_DOWN || i == VK_ESCAPE)
			{
				core::EventManager::create_event(event_type::EVENT_INPUT, i);
			}
		}


	}
}

