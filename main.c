#include <Windows.h>
#include <stdio.h>

#include "interface.h"
#include "snake.h"

//#pragma comment(lib,"user32.lib")

enum {
	t_red = 31,
	t_green = 32,
	t_yellow = 33,
	t_blue = 34,
	t_magenta = 35,
	t_cyan = 36,
	t_white = 37
};

int main(void)
{
	HANDLE out_handle;
	HANDLE in_handle;
	CONSOLE_SCREEN_BUFFER_INFO win_info;
	HWND win_handle;
	struct snake_t snake;
	int res, win_width, win_height;

	out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (out_handle == INVALID_HANDLE_VALUE) {
		wprintf_s(L"GetStdHandle\n");
		return GetLastError();
	}

	in_handle = GetStdHandle(STD_INPUT_HANDLE);
	if (in_handle == INVALID_HANDLE_VALUE) {
		wprintf_s(L"GetStdHandle\n");
		return GetLastError();
	}

	win_handle = GetConsoleWindow();
	if (!win_handle) {
		wprintf_s(L"GetConsoleWindow\n");
		return GetLastError();
	}

	res = term_setup(out_handle, win_handle);
	if (res) {
		wprintf_s(L"Error in term_setup function\n");
		return res;
	}

	GetConsoleScreenBufferInfo(out_handle, &win_info);
	win_width = win_info.srWindow.Right - win_info.srWindow.Left + 1;
	win_height = win_info.srWindow.Bottom - win_info.srWindow.Top + 1;
	
	// menu while
	while (menu(in_handle, win_width, win_height) == start_choice) {
		res = game_init(out_handle, &snake, win_width, win_height, t_yellow);
		if (res) {
			wprintf_s(L"Error in game_init function\n");
			return res;
		}
		// game while
		while ((game_controller(in_handle, &snake)) != game_over) {
			game_update(&snake);
		}
	}

	//Sleep(10000);
	return 0;
}
