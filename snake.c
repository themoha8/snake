#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include "snake.h"

enum {
	snake_speed = 10,
	esc_key = 27,
};

void draw_snake(const struct snake_t* snake)
{
	// set snake position
	wprintf_s(L"\x1b[%d;%dH", snake->coord_y, snake->coord_x);
	// set snake color
	wprintf_s(L"\x1b[%dm", snake->color);
	// draw snake
	putwchar(L'@');
}

void create_map(HANDLE out_handle, const struct win_settings_t *win_settings)
{
	int x, y;

	// clear screen
	wprintf_s(L"\x1b[2J");

	// set color yellow
	wprintf_s(L"\x1b[%dm", win_settings->map_color);

	// set position
	wprintf_s(L"\x1b[0;0H");

	for (y = 0; y < win_settings->win_height - 7; y++) {
		for (x = 0; x < win_settings->win_width; x++) {
			if (x == 0 || x == win_settings->win_width - 1 || y == 0 || y == win_settings->win_height - 8) {
				putwchar(L'#');
			}
			else
				putwchar(L' ');
		}
	}

	// draw help
	wprintf_s(L"\x1b[%d;%dH", win_settings->win_height-6, win_settings->win_width-24);
	wprintf_s(L"escape key - exit to menu");

	draw_score(0, win_settings->win_height, win_settings->score_color);
}
/*
int game_init(HANDLE out_handle, struct snake_t *snake, const struct win_settings_t* win_settings)
{
	create_map(out_handle, win_settings);

	// init snake
	snake->coord_x = win_settings->win_width / 2;
	snake->coord_y = win_settings->win_height / 2;
	snake->direction = stop;
	snake->speed = 100;
	snake->color = snake->color;

	draw_snake(snake);

	return ERROR_SUCCESS;
}
*/
void draw_score(int value, int win_height, int score_color)
{
	// set cursor position
	wprintf_s(L"\x1b[%d;0H", win_height-6);
	// set color
	wprintf_s(L"\x1b[%dm", score_color);
	// draw score
	wprintf_s(L"Score: %d", value);
}

void key_check(HANDLE in_handle, wchar_t *pressed_key)
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

/*
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
	draw_snake(snake);
	// draw score
	//score(0);

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
*/