/*
#ifndef INTERFACE_H_SENTRY
#define INTERFACE_H_SENTRY

...

#endif
*/
#pragma once

enum game_t {
	game_over = 0,
	playing = 1,
};

struct snake_t {
	int coord_x;
	int coord_y;
	wchar_t direction;
	int speed;
};

int game_init(HANDLE out_handle, struct snake_t* snake, int win_width, int win_height, int map_color);
enum game_t game_controller(HANDLE in_handle, struct snake_t *snake);
void game_update(struct snake_t* snake);
static void draw_score(int value, int win_height, int color);
static void key_check(HANDLE in_handle, wchar_t* pressed_key);
static void create_map(HANDLE out_handle, int win_width, int win_height, int map_color);
static void draw_snake(int x, int y);