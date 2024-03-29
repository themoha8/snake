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

int term_setup(HANDLE out_handle, HWND win_handle);
enum choice_t menu(HANDLE in_handle, int win_width, int win_height);
