#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "snake.h"

enum {
	snake_speed = 10
};

enum direction_t {
	stop = 0
};

int game_init(HANDLE out_handle, struct snake_t *snake)
{
	HANDLE file_handle;
	wchar_t map_buf[2048];
	DWORD num_of_bytes_read;

	file_handle = CreateFileW(L"map.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle == INVALID_HANDLE_VALUE) {
		return GetLastError();
	}

	// set color yellow
	wprintf_s(L"\x1b[33m");

	// set cursor position
	wprintf_s(L"\x1b[0;0H");

	// draw map
	while (ReadFile(file_handle, map_buf, 2048, &num_of_bytes_read, NULL) && num_of_bytes_read > 0) {
		WriteConsoleA(out_handle, map_buf, num_of_bytes_read, NULL, NULL);
	}
	CloseHandle(file_handle);

	// draw score
	score(0);

	// init snake
	snake->coord_x = 21 / 2;
	snake->coord_y = 80 / 2;
	snake->direction = stop;
	snake->speed = snake_speed;

	// set snake position
	wprintf_s(L"\x1b[%d;%dH", snake->coord_x, snake->coord_y);
	// set snake color
	wprintf_s(L"\x1b[36m");
	// draw snake
	putwchar(L'@');

	return ERROR_SUCCESS;
}

static void score(int value)
{
	// set cursor position
	wprintf_s(L"\x1b[22;0H");
	// draw score
	wprintf_s(L"Score: %d", value);
}

static void draw_snake(void)
{

}