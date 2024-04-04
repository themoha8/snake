#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include "snake.h"

enum {
	wall_height_shift = 3,
	//max_tail = 50,
	// ----------------------
	snake_speed = 10,
	esc_key = 27,
	// ----------------------
	t_black = 30,
	t_red = 31,
	t_green = 32,
	t_yellow = 33,
	t_blue = 34,
	t_magenta = 35,
	t_cyan = 36,
	t_white = 37,
	t_cyan_back = 106,
	t_white_back = 47
};

enum direction_t {
	stop = 0,
	crashed = 1
};

static void draw_snake(const struct snake_t* snake)
{
	// set cursor position
	wprintf_s(L"\x1b[%d;%dH", snake->coord_y, snake->coord_x);
	// set color
	wprintf_s(L"\x1b[%dm", snake->color);
	// draw snake head
	putwchar(L'o');
}

static void draw_score(int value, int win_height, int score_color)
{
	// set cursor position
	wprintf_s(L"\x1b[%d;0H", win_height);
	// set color (font)
	wprintf_s(L"\x1b[%dm", score_color);
	// set color (back)
	//wprintf_s(L"\x1b[%dm", t_white_back);
	// draw score
	wprintf_s(L"Score: %d", value);
	// set default color
	//wprintf_s(L"\x1b[0m");
}

static void draw_help(const struct win_settings_t* win_settings)
{
	// set color (back)
	wprintf_s(L"\x1b[%dm", t_cyan_back);
	// set color (front)
	wprintf_s(L"\x1b[%dm", t_black);
	// draw help
	wprintf_s(L"\x1b[%d;%dH", win_settings->win_height, (win_settings->win_width - 41) / 2);
	wprintf_s(L"Esc - back to the menu, P - pause the game");
	// set default color
	wprintf_s(L"\x1b[%dm", 0);
}

static void create_map(HANDLE out_handle, const struct win_settings_t* win_settings)
{
	int x, y;

	// clear screen
	wprintf_s(L"\x1b[2J");

	// set color yellow
	wprintf_s(L"\x1b[%dm", win_settings->map_color);

	// set cursor position
	wprintf_s(L"\x1b[0;0H");

	for (y = 0; y < win_settings->win_height - wall_height_shift; y++) {
		for (x = 0; x < win_settings->win_width; x++) {
			if (x == 0 || x == win_settings->win_width - 1 || y == 0 || y == win_settings->win_height - wall_height_shift - 1) {
				wprintf_s(L"\x1b[%dm", win_settings->map_color);
				putwchar(L'#');
			}
			else
				putwchar(L' ');
				/* {
				//if ((x + y) % 2) { // chest
				if(x % 2) { // beatifull
					wprintf_s(L"\x1b[%dm", 42);
					putwchar(L' ');
				}
				else {
					wprintf_s(L"\x1b[%dm", 102);
					putwchar(L' ');
				}
				wprintf_s(L"\x1b[%dm", 0);
			} */

		}
	}

	draw_help(win_settings);
	draw_score(0, win_settings->win_height - wall_height_shift + 1, win_settings->score_color);
}

static void draw_fruit(const struct fruit_t* fruit)
{
	// set cursor position
	wprintf_s(L"\x1b[%d;%dH", fruit->y, fruit->x);
	// set color
	wprintf_s(L"\x1b[%dm", fruit->color);
	// draw score
	putwchar(1);
}

static int check_fruit_on_tail(const struct snake_tail_t* snake_tail, struct fruit_t* fruit, int num_of_tail)
{
	if (num_of_tail > 0) {
		const struct snake_tail_t* tmp_snake_tail = snake_tail;
		while (tmp_snake_tail) {
			if (tmp_snake_tail->x == fruit->x && tmp_snake_tail->y == fruit->y)
				return 1;
			tmp_snake_tail = tmp_snake_tail->next;
		}
	}
	return 0;
}

static void create_fruit(struct fruit_t* fruit, int max_width, int max_height)
{
	srand((long)time(NULL));
	// double number in order to get an exact number 
//	random = rand() / (RAND_MAX + 1.0) * (max - min);
	fruit->x = (rand() % (max_width - 1)) + 2;
	fruit->y = (rand() % (max_height - 1)) + 2;
}

void game_init(HANDLE out_handle, struct snake_t* snake, const struct win_settings_t* win_settings, struct fruit_t* fruit)
{
	create_map(out_handle, win_settings);

	// init snake
	snake->coord_x = win_settings->win_width / 2;
	snake->coord_y = (win_settings->win_height - wall_height_shift) / 2;
	snake->direction = stop;
	snake->speed = 150;
	snake->color = t_red;
	snake->score = 0;
	snake->score_old = 0;
	snake->num_of_tail = 0;
	snake->acceleration = 0;

	// init food
	fruit->color = t_green;
	do {
		create_fruit(fruit, win_settings->win_width, win_settings->win_height - wall_height_shift - 1);
	} while (snake->coord_x == fruit->x && snake->coord_y == fruit->y);

	//draw_snake(snake);
	draw_score(snake->score, win_settings->win_height - wall_height_shift + 1, win_settings->score_color);
}

static void draw_snake_tail(const struct snake_tail_t** snake_tail, int color)
{
	const struct snake_tail_t* tmp_snake_tail = *snake_tail;
	// set color
	wprintf_s(L"\x1b[%dm", color);
	while (tmp_snake_tail) {
		if (tmp_snake_tail->x != 0 || tmp_snake_tail->y != 0) {
			// set position
			wprintf_s(L"\x1b[%d;%dH", tmp_snake_tail->y, tmp_snake_tail->x);
			if (tmp_snake_tail->direction == L'w' || tmp_snake_tail->direction == L's' || tmp_snake_tail->direction == L'W' || tmp_snake_tail->direction == L'S')
				putwchar(L'|');
			else if (tmp_snake_tail->direction == L'a' || tmp_snake_tail->direction == L'd' || tmp_snake_tail->direction == L'A' || tmp_snake_tail->direction == L'D')
				putwchar(L'-');
		}
		tmp_snake_tail = tmp_snake_tail->next;
	}
}

static void game_pause(HANDLE *out_handle, const struct win_settings_t *win_settings)
{
	wchar_t pressed_key = L'0';

	while (pressed_key != L'P') {
		// set color
		wprintf_s(L"\x1b[%dm", t_magenta);
		// set cursor position
		wprintf_s(L"\x1b[%d;%dH", (win_settings->win_height - wall_height_shift) / 2, (win_settings->win_width - 5) / 2);
		wprintf_s(L"PAUSE");
		Sleep(500);
		// set color
		wprintf_s(L"\x1b[%dm", t_blue);
		// set cursor position
		wprintf_s(L"\x1b[%d;%dH", (win_settings->win_height - wall_height_shift) / 2, (win_settings->win_width - 5) / 2);
		wprintf_s(L"pause");
		Sleep(500);
		if (_kbhit() != 0)
			pressed_key = _getwch();
	}
	wprintf_s(L"\x1b[%d;%dH", (win_settings->win_height - wall_height_shift) / 2, (win_settings->win_width - 5) / 2);
	wprintf_s(L"     ");
}

enum game_t game_controller(HANDLE in_handle, HANDLE out_handle, struct snake_t* snake, const struct win_settings_t* win_settings, struct fruit_t* fruit, struct snake_tail_t** snake_tail)
{
	wchar_t pressed_key = L'0';

	// key_event
	if (_kbhit() != 0)
		pressed_key = _getwch();

	if (pressed_key == esc_key || snake->direction == crashed) {
		if (snake->num_of_tail > 0) {
			while (*snake_tail) {
				struct snake_tail_t* tmp = *snake_tail;
				*snake_tail = (*snake_tail)->next;
				HeapFree(GetProcessHeap(), 0, tmp);
			}
		}
		PlaySoundW(L"res\\Lose.wav", NULL, SND_FILENAME | SND_ASYNC);
		Sleep(2500);
		return game_over;
	}
	else if (pressed_key == L'w' || pressed_key == L'W' ||
		pressed_key == L'a' || pressed_key == L'A' ||
		pressed_key == L's' || pressed_key == L'S' ||
		pressed_key == L'd' || pressed_key == L'D') {

		if (snake->direction == pressed_key && snake->acceleration < 100)
			snake->acceleration += 10;
		else if(snake->direction != pressed_key)
			snake->acceleration = 0;

		snake->direction = pressed_key;
	}
	else if(pressed_key == L'P') {
		draw_snake(snake);
		if (snake->num_of_tail > 0)
			draw_snake_tail(&(*snake_tail), snake->color);
		game_pause(out_handle, win_settings);
	}

	// draw_event
		// ---> draw snake
		// ---> draw score
		// ---> draw tail
		// ---> draw fruit
	draw_snake(snake);
	if (snake->score > snake->score_old) {
		draw_score(snake->score, win_settings->win_height - wall_height_shift + 1, win_settings->score_color);
		snake->score_old = snake->score;
	}
	draw_fruit(fruit);

	if (snake->num_of_tail > 0)
		draw_snake_tail(&(*snake_tail), snake->color);


	// step_event (game time step)
	// allows you to adjust the speed of the program.
	// eliminates flicker. 
	// frame rendering speed
	//Sleep(30);
	return playing;
}

// makes all changes in the game
void game_update(struct snake_t* snake, const struct win_settings_t* win_settings, struct fruit_t* fruit, struct snake_tail_t** snake_tail)
{
	short tmp_x, tmp_y, prev_tail_x, prev_tail_y, tmp_tail_x, tmp_tail_y;
	short prev_tail_direction, prev_tail_direction2;
	struct snake_tail_t* tmp_snake_tail;
	tmp_x = snake->coord_x;
	tmp_y = snake->coord_y;

	Sleep(snake->speed - snake->acceleration);
	// set cursor position
	wprintf_s(L"\x1b[%d;%dH", snake->coord_y, snake->coord_x);
	// clear snake
	putwchar(L' ');

	// catching a fruit
	if (snake->coord_x == fruit->x && snake->coord_y == fruit->y) {
		PlaySoundW(L"res\\Apple_bite.wav", NULL, SND_FILENAME | SND_ASYNC);
		Sleep(70);
		snake->num_of_tail++;
		snake->score++;
		if (snake->score % 5 == 0) {
			snake->speed = snake->speed - 10;
		}
		tmp_snake_tail = (struct snake_tail_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct snake_tail_t));
		tmp_snake_tail->next = *snake_tail;
		*snake_tail = tmp_snake_tail;
		
		do {
			create_fruit(fruit, win_settings->win_width, win_settings->win_height - wall_height_shift - 1);
		} while ((snake->coord_x == fruit->x && snake->coord_y == fruit->y) || check_fruit_on_tail(*snake_tail, fruit, snake->num_of_tail));
	}

	if (snake->num_of_tail > 0) {
		// the first element of the tail is assigned the values of the head
		// before this, we will save the coordinates of the first tail for transmission to the next element
		prev_tail_y = (*snake_tail)->y;
		prev_tail_x = (*snake_tail)->x;
		prev_tail_direction = (*snake_tail)->direction;
		(*snake_tail)->x = tmp_x;
		(*snake_tail)->y = tmp_y;
		(*snake_tail)->direction = snake->direction;
		// erase the first element of the tail
		// set cursor position
		if (prev_tail_x != 0 || prev_tail_y != 0) {
			wprintf_s(L"\x1b[%d;%dH", prev_tail_y, prev_tail_x);
			putwchar(L' ');
		}
		tmp_snake_tail = (*snake_tail)->next;
		while (tmp_snake_tail) {
			tmp_tail_x = tmp_snake_tail->x;
			tmp_tail_y = tmp_snake_tail->y;
			prev_tail_direction2 = tmp_snake_tail->direction;
			if (tmp_tail_y != 0 || tmp_tail_x != 0) {
				wprintf_s(L"\x1b[%d;%dH", tmp_tail_y, tmp_tail_x);
				putwchar(L' ');
			}
			tmp_snake_tail->x = prev_tail_x;
			tmp_snake_tail->y = prev_tail_y;
			tmp_snake_tail->direction = prev_tail_direction;
			prev_tail_x = tmp_tail_x;
			prev_tail_y = tmp_tail_y;
			prev_tail_direction = prev_tail_direction2;
			tmp_snake_tail = tmp_snake_tail->next;
		}
	}

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
	else if (snake->direction == L's' || snake->direction == L'S') {
		tmp_y++;
		if (tmp_y == win_settings->win_height - wall_height_shift) // wall
			snake->direction = crashed;
		else
			snake->coord_y = tmp_y;
	}
	else if (snake->direction == L'd' || snake->direction == L'D') {
		tmp_x++;
		if (tmp_x == win_settings->win_width) //wall
			snake->direction = crashed;
		else
			snake->coord_x = tmp_x;
	}

	// catching a tail
	if (snake->num_of_tail > 0) {
		tmp_snake_tail = *snake_tail;
		while (tmp_snake_tail) {
			if (snake->coord_x == tmp_snake_tail->x && snake->coord_y == tmp_snake_tail->y)
				snake->direction = crashed;
			tmp_snake_tail = tmp_snake_tail->next;
		}
	}
}

/*
static int check_coordinates(struct snake_t* snake, int max_map_width, int max_map_height)
{
	int tmp_x, tmp_y;

	tmp_x = snake->coord_x;
	tmp_y = snake->coord_y;

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
	else if (snake->direction == L's' || snake->direction == L'S') {
		tmp_y++;
		if (tmp_y == max_map_height) // wall
			snake->direction = crashed;
		else
			snake->coord_y = tmp_y;
	}
	else if (snake->direction == L'd' || snake->direction == L'D') {
		tmp_x++;
		if (tmp_x == max_map_width)
			snake->direction = crashed;
		else
			snake->coord_x = tmp_x;
	}
}
*/
