#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "interface.h"

#pragma comment(lib, "Winmm.lib")

enum {
	enter_key = 13,
};

int term_setup(HANDLE out_handle, HWND win_handle)
{
	DWORD out_cls_mode;

	// setup console (enable sequence control)
	if (!GetConsoleMode(out_handle, &out_cls_mode)) {
		return GetLastError();
	}

	out_cls_mode = out_cls_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	if (!SetConsoleMode(out_handle, out_cls_mode)) {
		return GetLastError();
	}

	// set console title
	wprintf_s(L"\x1b]0;Snake\x1b\x5c");

	// set max console size
	ShowWindow(win_handle, SW_MAXIMIZE);

	// hide cursor
	wprintf_s(L"\x1b[?25l");
	return ERROR_SUCCESS;
}

enum choice_t menu(HANDLE in_handle, const struct win_settings_t *win_settings)
{
	int start_color = win_settings->start_color;
	int exit_color = win_settings->exit_color;
	enum choice_t choice = start_choice;
	wchar_t pressed_key = L'0'; // undef
	INPUT_RECORD in_buf;
	DWORD num_of_events;

	// clear screen
	wprintf_s(L"\x1b[2J");

	while (pressed_key != enter_key) {
		// draw color
		wprintf_s(L"\x1b[%dm", start_color);
		// set cursor position
		wprintf_s(L"\x1b[%d;%dH", win_settings->win_height / 2, (win_settings->win_width - 5) / 2);
		// output text
		wprintf_s(L"Start");

		// draw color
		wprintf_s(L"\x1b[%dm", exit_color);
		// set cursor position
		wprintf_s(L"\x1b[%d;%dH", (win_settings->win_height / 2) + 2, (win_settings->win_width - 5) / 2);
		// output text
		wprintf_s(L"Exit");

		FlushConsoleInputBuffer(in_handle);
		ReadConsoleInputW(in_handle, &in_buf, 1, &num_of_events);
		pressed_key = in_buf.Event.KeyEvent.uChar.UnicodeChar;
		if ((pressed_key == L'w' || pressed_key == L'W') && choice == exit_choice) {
			choice--; // choice = start_choice
			start_color = win_settings->start_color;
			exit_color = win_settings->exit_color;
			PlaySoundW(L"res\\Click.wav", NULL, SND_FILENAME | SND_ASYNC);
			Sleep(200); // pause
		}
		else if ((pressed_key == L's' || pressed_key == L'S') && choice == start_choice) {
			choice++; // choice = exit_choice
			start_color = win_settings->exit_color;
			exit_color = win_settings->start_color;
			PlaySoundW(L"res\\Click.wav", NULL, SND_FILENAME | SND_ASYNC);
			Sleep(200); // pause
		}
	}
	return choice;
}
