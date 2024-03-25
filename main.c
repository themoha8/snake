#include <Windows.h>
#include <stdio.h>
#include "interface.h"

int main(void)
{
	int res = term_setup();
	if (res) {
		wprintf_s(L"Error in function term_setup");
		return res;
	}

	Sleep(10000);
	return 0;
}
