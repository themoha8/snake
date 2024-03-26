/*
#ifndef INTERFACE_H_SENTRY
#define INTERFACE_H_SENTRY

...

#endif
*/
#pragma once

struct snake_t {
	int coord_x;
	int coord_y;
	int direction;
	int speed;
};

int game_init(HANDLE out_handle, struct snake_t* snake);
static void score(int value);
