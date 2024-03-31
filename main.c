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
	HWND win_handle;
	struct fruit_t fruit;
	struct snake_t snake;
	struct win_settings_t win_settings;
	struct snake_tail_t *snake_tail = NULL;
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

	win_settings.start_color = t_cyan;
	win_settings.exit_color = t_white;
	win_settings.map_color = t_yellow;
	win_settings.panel_color = t_red;
	win_settings.score_color = t_magenta;

	// menu while
	while (menu(in_handle, out_handle, &win_settings) == start_choice) {
		game_init(out_handle, &snake, &win_settings, &fruit);
		// game while
		while ((game_controller2(in_handle, &snake, &win_settings, &fruit, &snake_tail)) != game_over) {
			game_update2(&snake, &win_settings, &fruit, &snake_tail);
		}
	}

	//Sleep(10000);

	return 0;
}
