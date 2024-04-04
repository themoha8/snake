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

struct fruit_t {
	short x;
	short y;
	short color;
};

struct snake_t {
	short coord_x;
	short coord_y;
	wchar_t direction;
	short speed;
	short color;
	short score;
	short score_old;
	short num_of_tail;
	short acceleration;
};

struct snake_tail_t {
	short x;
	short y;
	short direction;
	struct snake_tail_t* next;
};

void game_init(HANDLE out_handle, struct snake_t* snake, const struct win_settings_t* win_settings, struct fruit_t *fruit);
enum game_t game_controller(HANDLE in_handle, HANDLE out_handle, struct snake_t* snake, const struct win_settings_t* win_settings, struct fruit_t* fruit, struct snake_tail_t** snake_tail);
void game_update(struct snake_t* snake, const struct win_settings_t* win_settings, struct fruit_t* fruit, struct snake_tail_t** snake_tail);