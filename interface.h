/*
#ifndef INTERFACE_H_SENTRY
#define INTERFACE_H_SENTRY

...

#endif
*/
#pragma once

enum choice_t {
	error = 0,
	start_choice = 1,
	exit_choice = 2,
};

struct win_settings_t {
	short win_width;
	short win_height;
	short map_color;
	short start_color;
	short exit_color;
	short panel_color;
	short score_color;
};

int term_setup(HANDLE out_handle, HWND win_handle);
enum choice_t menu(HANDLE in_handle, const struct win_settings_t *win_settings);
