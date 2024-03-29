/*
#ifndef INTERFACE_H_SENTRY
#define INTERFACE_H_SENTRY

...

#endif
*/
#pragma once
#include "interface.h"

enum game_t {
	game_over = 0,
	playing = 1,
};

struct snake_t {
	short coord_x;
	short coord_y;
	wchar_t direction;
	short speed;
	short color;
};

enum direction_t {
	stop = 0,
	crashed = 1
};

//int game_init(HANDLE out_handle, struct snake_t* snake, const struct win_settings_t *win_settings);
//enum game_t game_controller(HANDLE in_handle, struct snake_t *snake);
//void game_update(struct snake_t* snake);
void draw_score(int value, int win_height, int score_color);
void key_check(HANDLE in_handle, wchar_t* pressed_key);
void create_map(HANDLE out_handle, const struct win_settings_t* win_settings);
void draw_snake(const struct snake_t *snake);