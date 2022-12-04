#include <Windows.h>
#include <stdio.h>
#include <cassert>
#include <memory>
#include <vector>

struct CHAR_LINE_INFO
{
	short CHAR_SOLID = 0x2588;
	int START_POSITION = 10;
	int CURRENT_POSITION = START_POSITION;
	int PRE_POSITION = -1;
	int MAX_LENGTH = 12;
	bool isStart = false;
};

int main()
{

	short m_WinSizeX = 100;
	short m_WinSizeY = 20;
	short m_FontSize = 12;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hConsole == INVALID_HANDLE_VALUE)
	{
		assert(hConsole == INVALID_HANDLE_VALUE);
		return 1;
	}

	CONSOLE_SCREEN_BUFFER_INFO csbInfo;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbInfo))
	{
		assert(false);
		return 1;
	}

	SMALL_RECT srctFullRect{ 0, 0, m_WinSizeX - 1, m_WinSizeY - 1 };
	if (!SetConsoleWindowInfo(hConsole, true, &srctFullRect))
	{
		assert(false);
		return 1;
	}

	COORD sizeXY = { m_WinSizeX, m_WinSizeY };
	if (!SetConsoleScreenBufferSize(hConsole, sizeXY))
	{
		assert(false);
		return 1;
	}

	if (!SetConsoleActiveScreenBuffer(hConsole))
	{
		assert(false);
		return 1;
	}

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = m_FontSize;
	cfi.dwFontSize.Y = m_FontSize * 2;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_BOLD;

	wcscpy_s(cfi.FaceName, L"Consolas");
	if (!SetCurrentConsoleFontEx(hConsole, false, &cfi))
	{
		assert(false);
		return 1;
	}

	std::vector<CHAR_INFO> m_BufScreen(m_WinSizeX * m_WinSizeY);
	m_BufScreen.resize(m_WinSizeX * m_WinSizeY);

	memset(&(*m_BufScreen.begin()), 0, sizeof(CHAR_INFO) * m_BufScreen.size());


	std::srand(GetTickCount());

	std::vector<CHAR_LINE_INFO> cli(m_WinSizeX * m_WinSizeY);
	for (auto i{ 0 }; i < cli.size(); ++i)
	{
		cli[i].START_POSITION = i;
		cli[i].CURRENT_POSITION = cli[i].START_POSITION;
		cli[i].isStart = false;
		cli[i].MAX_LENGTH = 8 + rand() % (m_WinSizeY - 8);
	}

	auto currStart = 0;

	CONSOLE_CURSOR_INFO curInfo;
	curInfo.dwSize = 1;
	curInfo.bVisible = false;

	if (!SetConsoleCursorInfo(hConsole, &curInfo))
	{
		assert(false);
		return 1;
	}

	while (true)
	{
		for (auto & line : cli)
		{
			auto &[CHAR_SOLID, START_POSITION, CURRENT_POSITION, PRE_POSITION, MAX_LENGTH, isStart] = line;

			if (!isStart && currStart < m_WinSizeX / 3)
			{
					isStart = rand() % 100000;
					isStart = !isStart;

					if (isStart)
					{
						++currStart;
					}
			}

			if (isStart)
			{

				if (PRE_POSITION != -1 && PRE_POSITION < m_WinSizeX * m_WinSizeY)
				{
					m_BufScreen.at(PRE_POSITION).Char.UnicodeChar = 0;
					m_BufScreen.at(PRE_POSITION).Char.AsciiChar = 33 + std::rand() % (126 - 33);
					m_BufScreen.at(PRE_POSITION).Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

				}
				if (CURRENT_POSITION < m_WinSizeX * m_WinSizeY)
				{

					(m_BufScreen.begin() + CURRENT_POSITION)->Char.UnicodeChar = CHAR_SOLID;
					m_BufScreen.at(CURRENT_POSITION).Attributes = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN;
				}

				PRE_POSITION = CURRENT_POSITION;
				CURRENT_POSITION += m_WinSizeX;

				const auto LAST_SYMBOL = CURRENT_POSITION - m_WinSizeX * MAX_LENGTH;
				if (LAST_SYMBOL >= START_POSITION && LAST_SYMBOL < m_WinSizeX * m_WinSizeY)
				{
					m_BufScreen.at(LAST_SYMBOL).Char.UnicodeChar = 0;
					m_BufScreen.at(LAST_SYMBOL).Attributes = 0;
				}
				if (CURRENT_POSITION - m_WinSizeX * (MAX_LENGTH - (MAX_LENGTH / 3)) >= START_POSITION && CURRENT_POSITION - m_WinSizeX * (MAX_LENGTH - (MAX_LENGTH / 3)) < m_WinSizeX * m_WinSizeY)
				{
					m_BufScreen.at(CURRENT_POSITION - m_WinSizeX * (MAX_LENGTH - (MAX_LENGTH / 3))).Attributes = FOREGROUND_GREEN;
				}

				if (CURRENT_POSITION - m_WinSizeX * (MAX_LENGTH - 1) >= START_POSITION && CURRENT_POSITION - m_WinSizeX * (MAX_LENGTH - 1) < m_WinSizeX * m_WinSizeY)
				{
					m_BufScreen.at(CURRENT_POSITION - m_WinSizeX * (MAX_LENGTH - 1)).Attributes = FOREGROUND_INTENSITY;
				}

				if (LAST_SYMBOL >= m_WinSizeX * m_WinSizeY - 1)
				{
					PRE_POSITION = -1;
					START_POSITION = rand() % m_WinSizeX;
					CURRENT_POSITION = START_POSITION;
					MAX_LENGTH = 8 + rand() % (m_WinSizeY - 8);
					//memset(&(*m_BufScreen.begin()), 0, sizeof(CHAR_INFO) * m_BufScreen.size());
				}
			}
		}
		WriteConsoleOutput(hConsole, &(*m_BufScreen.begin()), { m_WinSizeX, m_WinSizeY }, { 0,0 }, &srctFullRect);

		Sleep(50);
	}
	return 0;
}