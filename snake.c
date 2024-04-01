#include <Windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <conio.h>
#include "snake.h"

enum {
	wall_height_shift = 3,
	max_tail = 50,
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
	// set snake position
	wprintf_s(L"\x1b[%d;%dH", snake->coord_y, snake->coord_x);
	// set snake color
	wprintf_s(L"\x1b[%dm", snake->color);
	// draw snake
	putwchar(L'@');
}

static void draw_score(int value, int win_height, int score_color)
{
	// set cursor position
	wprintf_s(L"\x1b[%d;0H", win_height);
	// set color (font)
	wprintf_s(L"\x1b[%dm", score_color);
	// set coloor (back)
	wprintf_s(L"\x1b[%dm", t_white_back);
	// draw score
	wprintf_s(L"Score: %d", value);
	// set default color
	wprintf_s(L"\x1b[0m");
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
	wprintf_s(L"\x1b[%dm", 0);
}

static void create_map(HANDLE out_handle, const struct win_settings_t *win_settings)
{
	int x, y;

	// clear screen
	wprintf_s(L"\x1b[2J");

	// set color yellow
	wprintf_s(L"\x1b[%dm", win_settings->map_color);

	// set position
	wprintf_s(L"\x1b[0;0H");

	for (y = 0; y < win_settings->win_height - wall_height_shift; y++) {
		for (x = 0; x < win_settings->win_width; x++) {
			if (x == 0 || x == win_settings->win_width - 1 || y == 0 || y == win_settings->win_height - wall_height_shift - 1) {
				putwchar(L'#');
			}
			else
				putwchar(L' ');
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

static void create_fruit(struct fruit_t* fruit)
{
	fruit->x = 15;
	fruit->y = 15;
	fruit->color = t_green;
}

void game_init(HANDLE out_handle, struct snake_t *snake, const struct win_settings_t* win_settings, struct fruit_t *fruit)
{
	create_map(out_handle, win_settings);

	// init snake
	snake->coord_x = win_settings->win_width / 2;
	snake->coord_y = (win_settings->win_height - wall_height_shift) / 2;
	snake->direction = stop;
	snake->speed = 200;
	snake->color = t_red;
	snake->score = 0;
	snake->score_old = 0;
	snake->num_of_tail = 0;

	// init food
	create_fruit(fruit);

	draw_snake(snake);
	draw_score(snake->score, win_settings->win_height - wall_height_shift + 1, win_settings->score_color);
}

static struct snake_tail_t *inc_tail(void)
{
	struct snake_tail_t *tmp = (struct snake_tail_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct snake_tail_t));
	return tmp;
}

static void draw_snake_tail2(const struct snake_tail_t** snake_tail, int color)
{
	const struct snake_tail_t* tmp_snake_tail = *snake_tail;
	// set color
	wprintf_s(L"\x1b[%dm", color);
	while (tmp_snake_tail) {
		// set position
		if (tmp_snake_tail->x != 0 || tmp_snake_tail->y != 0) {
			wprintf_s(L"\x1b[%d;%dH", tmp_snake_tail->y, tmp_snake_tail->x);
			putwchar(L'o');
		}
		tmp_snake_tail = tmp_snake_tail->next;
	}
}

enum game_t game_controller2(HANDLE in_handle, struct snake_t* snake, const struct win_settings_t* win_settings, struct fruit_t* fruit, struct snake_tail_t** snake_tail)
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
		return game_over;
	}
	else if (pressed_key == L'w' || pressed_key == L'W' ||
		pressed_key == L'a' || pressed_key == L'A' ||
		pressed_key == L's' || pressed_key == L'S' ||
		pressed_key == L'd' || pressed_key == L'D') {

		snake->direction = pressed_key;
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
		draw_snake_tail2(&(*snake_tail), snake->color);


	// step_event (game time step)
	// allows you to adjust the speed of the program.
	// eliminates flicker. 
	// frame rendering speed
	//Sleep(30);
	return playing;
}

// makes all changes in the game
void game_update2(struct snake_t* snake, const struct win_settings_t* win_settings, struct fruit_t* fruit, struct snake_tail_t** snake_tail)
{
	short tmp_x, tmp_y, prev_tail_x, prev_tail_y, tmp_tail_x, tmp_tail_y;
	struct snake_tail_t* tmp_snake_tail;
	tmp_x = snake->coord_x;
	tmp_y = snake->coord_y;

	Sleep(snake->speed);
	// set cursor position
	wprintf_s(L"\x1b[%d;%dH", snake->coord_y, snake->coord_x);
	// clear snake
	putwchar(L' ');

	/*
	// catching a fruit
	if (snake->coord_x == fruit->x && snake->coord_y == fruit->y) {
		//	if(snake->num_of_tail < max_tail)
		snake->num_of_tail++;
		snake->score++;
		if (snake->num_of_tail == 1) {
			*snake_tail = inc_tail();
			(*snake_tail)->last = *snake_tail;
		}
		else
			//(*snake_tail)->last = inc_tail();
	*/
	// catching a fruit
	if (snake->coord_x == fruit->x && snake->coord_y == fruit->y) {
		snake->num_of_tail++;
		snake->score++;
		tmp_snake_tail = (struct snake_tail_t*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(struct snake_tail_t));
		tmp_snake_tail->next = *snake_tail;
		*snake_tail = tmp_snake_tail;
		create_fruit(fruit);
	}

	if (snake->num_of_tail > 0) {
		// the first element of the tail is assigned the values of the head
		// before this, we will save the coordinates of the first tail for transmission to the next element
		prev_tail_y = (*snake_tail)->y;
		prev_tail_x = (*snake_tail)->x;
		(*snake_tail)->x = tmp_x;
		(*snake_tail)->y = tmp_y;
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
			if (tmp_tail_y != 0 || tmp_tail_x != 0) {
				wprintf_s(L"\x1b[%d;%dH", tmp_tail_y, tmp_tail_x);
				putwchar(L' ');
			}
			tmp_snake_tail->x = prev_tail_x;
			tmp_snake_tail->y = prev_tail_y;
			prev_tail_x = tmp_tail_x;
			prev_tail_y = tmp_tail_y;
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
		if (tmp_x == win_settings->win_width)
			snake->direction = crashed;
		else
			snake->coord_x = tmp_x;
	}

	// catching a tail
	if (snake->num_of_tail > 0) {
		tmp_snake_tail = *snake_tail;
		while (tmp_snake_tail) {
			if (snake->coord_x == (*snake_tail)->x && snake->coord_y == (*snake_tail)->y)
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

static struct snake_str* inc_tail(const struct snake_str* lsnake, const struct map_str* map)
{
	struct snake_str* tmp = malloc(sizeof(struct snake_str));
	tmp->x = lsnake->x;
	tmp->y = lsnake->y;
	tmp->dx = lsnake->dx;
	tmp->dy = lsnake->dy;
	tmp->dir = lsnake->dir;
	tmp->next = NULL;

	tmp->x -= tmp->dx;
	check(&(tmp->x), map->max_x);
	tmp->y -= tmp->dy;
	check(&(tmp->y), map->max_y);

	return tmp;
}


// makes all changes in the game
void game_update2(struct snake_t* snake, const struct win_settings_t* win_settings)
{
	//int tmp_x, tmp_y;

	//tmp_x = snake->coord_x;
//	tmp_y = snake->coord_y;

	Sleep(snake->speed);
	// set cursor position
	wprintf_s(L"\x1b[%d;%dH", snake->coord_y, snake->coord_x);
	// clear snake
	putwchar(L' ');

	check_coordinates(snake, win_settings->win_width, win_settings->win_height - wall_height_shift);
			/*
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
				if (tmp_y == win_settings->win_height - wall_height_shift) // wall
					snake->direction = crashed;
				else
					snake->coord_y = tmp_y;
			}
			else if (snake->direction == L'd' || snake->direction == L'D') {
				tmp_x++;
				if (tmp_x == win_settings->win_width)
					snake->direction = crashed;
				else
					snake->coord_x = tmp_x;
			}
			
}
*/
