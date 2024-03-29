#include <Windows.h>
#include <stdio.h>

#include "interface.h"
#include "snake.h"

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
	struct win_settings_t win_settings;
	int res, i = 0;

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
	win_settings.win_width = win_info.srWindow.Right - win_info.srWindow.Left + 1;
	win_settings.win_height = win_info.srWindow.Bottom - win_info.srWindow.Top + 1;
	win_settings.start_color = t_cyan;
	win_settings.exit_color = t_white;
	win_settings.map_color = t_yellow;
	win_settings.panel_color = t_red;
	win_settings.score_color = t_magenta;

	snake.coord_x = win_settings.win_width / 2;
	snake.coord_y = (win_settings.win_height-8) / 2;
	snake.direction = stop;
	snake.speed = 100;
	snake.color = t_red;

	// menu while
	while (menu(in_handle, &win_settings) == start_choice) {
		create_map(out_handle, &win_settings);
		draw_snake(&snake);
		wprintf_s(L"%d\n", i++);
		Sleep(2000);
		/*
		res = game_init(out_handle, &snake, &win_settings);
		if (res) {
			wprintf_s(L"Error in game_init function\n");
			return res;
		}
		// game while
		while ((game_controller(in_handle, &snake)) != game_over) {
			game_update(&snake);
		}
		*/
	}

	//Sleep(10000);

	return 0;
}
