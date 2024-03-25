#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "interface.h"

enum {
	//---------------------------
	left_x = 200,
	left_y = 50,
	right_x = 920,
	right_y = 650,
//---------------------------
	esc_key = 27,
	enter_key = 13,
	undef_key = -1,
//---------------------------
	t_red = 31,
	t_green = 32,
	t_blue = 34,
	t_magenta = 35,
	t_white = 37,
//---------------------------
	start_choice = 0,
	end_choice = 1
//---------------------------
};

int term_setup(void)
{
	HANDLE out_handle;
	DWORD out_cls_mode;
	HWND h_wnd;

	out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (out_handle == INVALID_HANDLE_VALUE) {
		return GetLastError();
	}

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

	// set console size and move window (deprecated functions)
	h_wnd = GetConsoleWindow();
	MoveWindow(h_wnd, left_x, left_y, right_x, right_y, true);

	// hide cursor
	wprintf_s(L"\x1b[?25l");
	return ERROR_SUCCESS;
}
