#include <Windows.h>
#include <stdio.h>

#include "interface.h"
#include "snake.h"

int main(void)
{
	struct snake_t snake;
	HANDLE out_handle;
	HANDLE in_handle;
	int res;

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

	res = term_setup(out_handle);
	if (term_setup(out_handle)) {
		wprintf_s(L"Error in term_setup function\n");
		return res;
	}
	// menu while
	while (menu(in_handle) == start_choice) {
		res = game_init(out_handle, &snake);
		if (res) {
			wprintf_s(L"Error in game_init function\n");
			return res;
		}
		// game while
		while ((game_controller(in_handle, &snake)) != game_over) {
			game_update(&snake);
		}
	}

//	Sleep(10000);
	return 0;
}
