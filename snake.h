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
	int coord_x;
	int coord_y;
	int direction;
	int speed;
};

int game_init(HANDLE out_handle, struct snake_t* snake);
enum game_t game_controller(HANDLE in_handle);
static void score(int value);
static void key_check(HANDLE in_handle, wchar_t* pressed_key);