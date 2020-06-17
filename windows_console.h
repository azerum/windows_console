#ifndef WINDOWS_CONSOLE_H
#define WINDOWS_CONSOLE_H
#pragma once

#include <Windows.h>

namespace console
{
	void showCursor(bool beVisible);

	void setCursorPosition(COORD coord);
	COORD getCursorPosition();

	bool setTextFont(const wchar_t *fontName, short charWidth, short charHeight);

	enum Color16
	{
		Black, Blue, Green, Cyan, Red, Magenta, Brown, LightGray, DarkGray,
		LightBlue, LightGreen, LightCyan, LightRed, LightMagenta, Yellow, White
	};

	struct TextColor
	{
		Color16 characters;
		Color16 background;
	};

	void setTextColor(TextColor color);
	void setTextColor(Color16 charactersColor);

	TextColor getTextColor();

	void setWindowSize(short width, short height);
	COORD getWindowSize();
}

#endif
