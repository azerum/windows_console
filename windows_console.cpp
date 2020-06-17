#include "windows_console.h"
#include <iostream>

//----------------------------------------------------------------------------
static const HANDLE getConsoleOutputHandle()
{
	static const HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	return hConsoleOutput;
}

//-----------------------------------------------------------------------------
void console::showCursor(bool beVisible)
{
	HANDLE hConsoleOutput = getConsoleOutputHandle();

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsoleOutput, &cursorInfo);

	cursorInfo.bVisible = beVisible;
	SetConsoleCursorInfo(hConsoleOutput, &cursorInfo);
}

//------------------------------------------------------------------------------
void console::setCursorPosition(COORD coord)
{
	std::cout.flush();
	SetConsoleCursorPosition(getConsoleOutputHandle(), coord);
}

//-------------------------------------------------------------------------------
COORD console::getCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(getConsoleOutputHandle(), &screenInfo);

	return screenInfo.dwCursorPosition;
}

//------------------------------------------------------------------------------------------
bool console::setTextFont(const wchar_t *fontName, short charWidth, short charHeight)
{
	HANDLE hConsoleOutput = getConsoleOutputHandle();

	CONSOLE_FONT_INFOEX fontInfo;

	fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	GetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo);

	fontInfo.dwFontSize.X = charWidth;
	fontInfo.dwFontSize.Y = charHeight;

	fontInfo.FontFamily = FF_DONTCARE;
	fontInfo.FontWeight = FW_NORMAL;

	static const size_t faceSize = sizeof(fontInfo.FaceName) / sizeof(fontInfo.FaceName[0]);

	wcscpy_s(fontInfo.FaceName, faceSize, fontName);
	return SetCurrentConsoleFontEx(hConsoleOutput, FALSE, &fontInfo) != 0;
}

//--------------------------------------------------------------------------------------------------------
console::TextColor console::getTextColor()
{
	TextColor textColor;

	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(getConsoleOutputHandle(), &screenInfo);

	textColor.background = static_cast<Color16>(screenInfo.wAttributes >> 4);
	textColor.characters = static_cast<Color16>(screenInfo.wAttributes - (screenInfo.wAttributes >> 4));

	return textColor;
}

//---------------------------------------------------------------------------------------------------------
void console::setTextColor(console::TextColor color)
{
	SetConsoleTextAttribute(getConsoleOutputHandle(), 
		static_cast<WORD>((color.background << 4) | color.characters)
	);
}

//----------------------------------------------------------------
void console::setTextColor(Color16 charactersColor)
{
	TextColor currentTextColor = getTextColor();
	TextColor newTextColor = { charactersColor, currentTextColor.background };

	console::setTextColor(newTextColor);
}

//--------------------------------------------------------------------------------------
void console::setWindowSize(short width, short height)
{
	HANDLE hConsoleOutput = getConsoleOutputHandle();

	static const COORD maxWindowSize = GetLargestConsoleWindowSize(hConsoleOutput);

	if (width > maxWindowSize.X) width = maxWindowSize.X;
	if (width <= 0) width = 1;

	if (height > maxWindowSize.Y) height = maxWindowSize.Y;
	if (height <= 0) height = 1;

	SMALL_RECT rect;
	rect.Top = 0;
	rect.Left = 0;
	rect.Bottom = height - 1;
	rect.Right = width - 1;

	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screenInfo);

	screenInfo.dwSize = { width, height };
	SetConsoleScreenBufferSize(hConsoleOutput, screenInfo.dwSize);

	SetConsoleWindowInfo(hConsoleOutput, TRUE, &rect);
}

//-----------------------------------------------------------------------------
COORD console::getWindowSize()
{
	HANDLE hConsoleOutput = getConsoleOutputHandle();

	CONSOLE_SCREEN_BUFFER_INFO screenInfo;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screenInfo);

	return screenInfo.dwSize;
}