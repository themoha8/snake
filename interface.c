#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include "interface.h"

#pragma comment(lib, "Winmm.lib")

enum {
	enter_key = 13,
	// ----------------------
	t_black = 30,
	t_red = 31,
	t_green = 32,
	t_yellow = 33,
	t_blue = 34,
	t_magenta = 35,
	t_cyan = 36,
	t_white = 37
};

int term_setup(HANDLE out_handle, HWND win_handle)
{
	DWORD out_cls_mode;

	// setup console (enable sequence control)
	if (!GetConsoleMode(out_handle, &out_cls_mode)) {
		return GetLastError();
	}

	out_cls_mode = out_cls_mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	if (!SetConsoleMode(out_handle, out_cls_mode)) {
		return GetLastError();
	}

	// set console title
	wprintf_s(L"\x1b]0;Snake\x1b\x5c");

	// set max console size
	ShowWindow(win_handle, SW_MAXIMIZE);

	return ERROR_SUCCESS;
}

static int check_window_size(const struct win_settings_t* win_settings) {
	if (win_settings->win_width < 40 || win_settings->win_height < 15) {
		// clear screen
		wprintf_s(L"\x1b[2J");
		// hide cursor
		wprintf_s(L"\x1b[?25l");
		// set color
		wprintf_s(L"\x1b[%dm", win_settings->map_color);
		// set cursor position
		wprintf_s(L"\x1b[0;0H");
		wprintf_s(L"Increase window size!\n");
		Sleep(2000);
		return 0;
	}
	return 1;
}

static void draw_logo(const struct win_settings_t* win_settings, int color)
{
	wprintf_s(L"\x1b[%dm", color);
	wprintf_s(L"\x1b[%d;%dH", win_settings->win_height / 3, (win_settings->win_width - 33) / 2);
	wprintf_s(L"  ____                 _");
	wprintf_s(L"\x1b[%d;%dH", (win_settings->win_height / 3) + 1, (win_settings->win_width - 33) / 2);
	wprintf_s(L" / ___|  _ __    __ _ | | __ ___");
	wprintf_s(L"\x1b[%d;%dH", (win_settings->win_height / 3) + 2, (win_settings->win_width - 33) / 2);
	wprintf(L" \\___ \\ |  _ \\  / _  || |/ // _ \\");
	wprintf_s(L"\x1b[%d;%dH", (win_settings->win_height / 3) + 3, (win_settings->win_width - 33) / 2);
	wprintf_s(L"  ___) || | | || (_| ||   <|  __/");
	wprintf_s(L"\x1b[%d;%dH", (win_settings->win_height / 3) + 4, (win_settings->win_width - 33) / 2);
	wprintf_s(L" |____/ |_| |_| \\____||_|\\_\\\\___|");
}

static void about_menu(HANDLE out_handle)
{
	HANDLE file_handle;
	char buf[2048];
	DWORD num_of_bytes_read;

	file_handle = CreateFileW(L"about.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file_handle == INVALID_HANDLE_VALUE) {
		// set cursor position
		wprintf_s(L"\x1b[0;0H");
		wprintf_s(L"read error: file - about.txt in about_menu function");
		return;
	}

	// clear screen
	wprintf_s(L"\x1b[2J");

	// set cursor position
	wprintf_s(L"\x1b[0;0H");

	// set color
	wprintf_s(L"\x1b[%dm", t_white);

	while (ReadFile(file_handle, buf, 2048, &num_of_bytes_read, NULL) && num_of_bytes_read > 0) {
		WriteConsoleA(out_handle, buf, num_of_bytes_read, NULL, NULL);
	}

	CloseHandle(file_handle);
	system("pause");
	PlaySoundW(L"res\\Click.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sleep(200); // pause
}

enum choice_t menu(HANDLE in_handle, HANDLE out_handle, struct win_settings_t* win_settings)
{
	int tmp_win_width, tmp_win_height;
	int start_color = win_settings->choice_color;
	int exit_color = win_settings->no_choice_color;
	int about_color = win_settings->no_choice_color;
	enum choice_t choice = start_choice;
	wchar_t pressed_key = L'0'; // undef
	INPUT_RECORD in_buf;
	DWORD num_of_events;
	CONSOLE_SCREEN_BUFFER_INFO win_info;
	
	// clear screen 
	wprintf_s(L"\x1b[2J");

	// hide cursor
	wprintf_s(L"\x1b[?25l");

	GetConsoleScreenBufferInfo(out_handle, &win_info);
	win_settings->win_width = win_info.srWindow.Right - win_info.srWindow.Left + 1;
	win_settings->win_height = win_info.srWindow.Bottom - win_info.srWindow.Top + 1;
	draw_logo(win_settings, t_green);

	while (pressed_key != enter_key) {
		
		GetConsoleScreenBufferInfo(out_handle, &win_info);
		tmp_win_width = win_settings->win_width;
		tmp_win_height = win_settings->win_height;
		win_settings->win_width = win_info.srWindow.Right - win_info.srWindow.Left + 1;
		win_settings->win_height = win_info.srWindow.Bottom - win_info.srWindow.Top + 1;

		if (!check_window_size(win_settings))
			continue;

		if (tmp_win_width != win_settings->win_width || tmp_win_height != win_settings->win_height) {
			// clear screen
			wprintf_s(L"\x1b[2J");

			// hide cursor
			wprintf_s(L"\x1b[?25l");

			draw_logo(win_settings, t_green);
		}

		// set color
		wprintf_s(L"\x1b[%dm", start_color);
		// set cursor position
		wprintf_s(L"\x1b[%d;%dH", win_settings->win_height / 2, (win_settings->win_width - 5) / 2);
		// output text
		wprintf_s(L"Start");

		// set color
		wprintf_s(L"\x1b[%dm", exit_color);
		// set cursor position
		wprintf_s(L"\x1b[%d;%dH", (win_settings->win_height / 2) + 2, (win_settings->win_width - 5) / 2);
		// output text
		wprintf_s(L"Exit");

		// set color
		wprintf_s(L"\x1b[%dm", about_color);
		// set cursor position
		wprintf_s(L"\x1b[%d;%dH", (win_settings->win_height / 2) + 4, (win_settings->win_width - 5) / 2);
		// output text
		wprintf_s(L"About");

		FlushConsoleInputBuffer(in_handle);
		ReadConsoleInputW(in_handle, &in_buf, 1, &num_of_events);
		pressed_key = in_buf.Event.KeyEvent.uChar.UnicodeChar;
		if ((pressed_key == L'w' || pressed_key == L'W') && choice == exit_choice) {
			choice--; // choice = start_choice
			start_color = win_settings->choice_color;
			exit_color = win_settings->no_choice_color;
			about_color = win_settings->no_choice_color;
			PlaySoundW(L"res\\Click.wav", NULL, SND_FILENAME | SND_ASYNC);
			Sleep(200); // pause
		}
		else if ((pressed_key == L's' || pressed_key == L'S') && choice == exit_choice) {
			choice++; // choice = about_choice
			start_color = win_settings->no_choice_color;
			exit_color = win_settings->no_choice_color;
			about_color = win_settings->choice_color;
			PlaySoundW(L"res\\Click.wav", NULL, SND_FILENAME | SND_ASYNC);
			Sleep(200); // pause
		}
		else if ((pressed_key == L's' || pressed_key == L'S') && choice == start_choice) {
			choice++; // choice = exit_choice
			start_color = win_settings->no_choice_color;
			exit_color = win_settings->choice_color;
			about_color = win_settings->no_choice_color;
			PlaySoundW(L"res\\Click.wav", NULL, SND_FILENAME | SND_ASYNC);
			Sleep(200); // pause
		}
		else if ((pressed_key == L'w' || pressed_key == L'W') && choice == about_choice) {
			choice--; // choice = exit_choice
			start_color = win_settings->no_choice_color;
			exit_color = win_settings->choice_color;
			about_color = win_settings->no_choice_color;
			PlaySoundW(L"res\\Click.wav", NULL, SND_FILENAME | SND_ASYNC);
			Sleep(200); // pause
		}
	}
	PlaySoundW(L"res\\Click.wav", NULL, SND_FILENAME | SND_ASYNC);
	Sleep(200);
	if (choice == about_choice) {
		about_menu(out_handle);
		//choice -= 2;
	}
	return choice;
}
