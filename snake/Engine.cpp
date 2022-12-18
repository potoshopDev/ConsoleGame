// Copyright © 2022 Oleg Belekhov. All Right Reserved.
// Licensed under the CC0 License
#include "Engine.h"
#include <cassert>

bool core::Engine::InitConsole() noexcept
{
	console_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (console_handle == INVALID_HANDLE_VALUE)
	{
		assert(INVALID_HANDLE_VALUE);
		return false;
	}

	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
	if (!GetConsoleScreenBufferInfo(console_handle, &csbInfo))
	{
		assert(false);
		return false;
	}
	
	full_rect = csbInfo.srWindow;
	if (!SetConsoleWindowInfo(console_handle, true, &full_rect))
	{
		assert(false);
		return false;
	}

	if (!SetConsoleScreenBufferSize(console_handle, buffer_size))
	{
		assert(false);
		return false;
	}

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.dwFontSize.X = core::g_FontSirze;
	cfi.dwFontSize.Y = core::g_FontSirze * 2;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	wcscpy_s(cfi.FaceName, L"Consolas");

	if (!SetCurrentConsoleFontEx(console_handle, false, &cfi))
	{
		assert(false);
		return false;
	}

	CONSOLE_CURSOR_INFO cci;
	cci.bVisible = false;
	cci.dwSize = 1;
	if (!SetConsoleCursorInfo(console_handle, &cci))
	{
		assert(false);
		return false;
	}


	CHAR_INFO ci;
	ci.Attributes = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
	ci.Char.UnicodeChar = 0;
	data__.char_buffer.assign(g_WinSizeX * g_WinSizeY, ci);

	buffer_size = csbInfo.dwSize;

	//disable resize console window 
	HWND consoleWindow = GetConsoleWindow();
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	/// ////////////////////////////////////

	return true;
}

void core::Engine::ErrorManager(int code_error)
{
	work_correct = false;
	assert(0 || "code_error = " || code_error);
}

void core::Engine::InputManager(int code_key)
{
	if (code_key == VK_ESCAPE)
	{
		work_correct = false;
	}
}

void core::Engine::Update_Draw() noexcept
{
	while (work_correct)
	{
		core::EventManager::create_event(emodule::event_type::EVENT_UPDATE);
		core::EventManager::call();
		Draw();
	}
}

void core::Engine::Draw() noexcept
{
	WriteConsoleOutput(console_handle, &(data__.char_buffer.front()), buffer_size, { 0, 0 }, &full_rect);
}
