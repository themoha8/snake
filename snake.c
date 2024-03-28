#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "snake.h"

enum {
	snake_speed = 10,
	esc_key = 27
};

enum direction_t {
	stop = 0,
	crashed = 1
};

static void draw_snake(int x, int y)
{
	// set snake position
	wprintf_s(L"\x1b[%d;%dH", y, x);
	// set snake color
	wprintf_s(L"\x1b[36m");
	// draw snake
	putwchar(L'@');
}

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

	// draw help
	wprintf_s(L"\x1b[22;55H");
	wprintf_s(L"escape key - exit to menu");

	// init snake
	snake->coord_x = 80 / 2;
	snake->coord_y = 21 / 2;
	snake->direction = stop;
	snake->speed = 100; //snake_speed;

	draw_snake(snake->coord_x, snake->coord_y);

	return ERROR_SUCCESS;
}

static void score(int value)
{
	// set cursor position
	wprintf_s(L"\x1b[22;0H");
	// set color yellow
	wprintf_s(L"\x1b[33m");
	// draw score
	wprintf_s(L"Score: %d", value);
}

static void key_check(HANDLE in_handle, wchar_t *pressed_key)
{
	INPUT_RECORD in_buf;
	DWORD num_of_events;

	// clear key
	*pressed_key = L'0';

	PeekConsoleInputW(in_handle, &in_buf, 1, &num_of_events);
	if (num_of_events > 0) {
		ReadConsoleInputW(in_handle, &in_buf, 1, &num_of_events);
		*pressed_key = in_buf.Event.KeyEvent.uChar.UnicodeChar;
	}

}

enum game_t game_controller(HANDLE in_handle, struct snake_t *snake)
{
	wchar_t pressed_key;

	// key_event
	key_check(in_handle, &pressed_key);
	if (pressed_key == esc_key) {
		return game_over;
	}
	else if (pressed_key == L'w' || pressed_key == L'W' ||
		pressed_key == L'a' || pressed_key == L'A' ||
		pressed_key == L's' || pressed_key == L'S' ||
		pressed_key == L'd' || pressed_key == L'D') {

		snake->direction = pressed_key;
	}

	// draw_event (redraws the snake)
		// ---> draw snake
		// ---> draw score
		// ---> draw panel
		
	// draw snake
	draw_snake(snake->coord_x, snake->coord_y);
	// draw score
	score(0);

	// step_event (game time step)
	// allows you to adjust the speed of the program.
	// eliminates flicker. 
	// frame rendering speed
	if (snake->direction == crashed) {
		return game_over;
	}
	Sleep(16);
	return playing;
}

// makes all changes in the game
void game_update(struct snake_t *snake)
{
	int tmp_x, tmp_y;

	tmp_x = snake->coord_x;
	tmp_y = snake->coord_y;

	Sleep(snake->speed); // own snake speed
	// set cursor position
	wprintf_s(L"\x1b[%d;%dH", snake->coord_y, snake->coord_x);
	// clear snake
	putwchar(L' ');
	Sleep(2000);
	if (snake->direction == L'w' || snake->direction == L'W') {
		tmp_y--;
		if (tmp_y == 1)
			snake->direction = crashed;
		else
			snake->coord_y = tmp_y;
	}
	else if (snake->direction == L'a' || snake->direction == L'A') {
		tmp_x--;
		if (tmp_x == 1) // wall
			snake->direction = crashed;
		else
			snake->coord_x = tmp_x;
	}
	else if(snake->direction == L's' || snake->direction == L'S') {
		tmp_y++;
		if (tmp_y == 21) // wall
			snake->direction = crashed;
		else
			snake->coord_y = tmp_y;
	}
	else if (snake->direction == L'd' || snake->direction == L'D') {
		tmp_x++;
		if (tmp_x == 79)
			snake->direction = crashed;
		else
			snake->coord_x = tmp_x;
	}
}