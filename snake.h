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
	short score;
};

void game_init(HANDLE out_handle, struct snake_t* snake, const struct win_settings_t *win_settings);
enum game_t game_controller(HANDLE in_handle, struct snake_t *snake, const struct win_settings_t* win_settings);
void game_update(struct snake_t* snake, const struct win_settings_t* win_settings);
