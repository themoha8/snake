#include <Windows.h>
#include <stdio.h>
#include "interface.h"
#include "snake.h"

int main(void)
{
	enum choice_t choice;
	struct snake_t snake;
	HANDLE out_handle;
	HANDLE in_handle;

	out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (out_handle == INVALID_HANDLE_VALUE) {
		wprintf_s(L"GetStdHandle\n");
		return GetLastError();
	}

	in_handle = GetStdHandle(STD_INPUT_HANDLE);
	if (in_handle == INVALID_HANDLE_VALUE) {
		wprintf_s(L"GetStdHandle\n");
		return GetLastError();
	}

	int res = term_setup(out_handle);
	if (res) {
		wprintf_s(L"Error in function term_setup\n");
		return res;
	}
	choice = menu(in_handle);

	//while ((choice = menu()) != exit_choice) {
	if(choice == start_choice)
		game_init(out_handle, &snake);
	//}
	Sleep(10000);
	return 0;
}
