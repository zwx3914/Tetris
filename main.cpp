#include "Tetris.h"
#include <iostream>
#include <cstdlib>
#include <windows.h>

int main()
{
	initialized();
	
	while (game_flag)
	{
		updateWithoutInput();
		updateWithInput();
		show();
		Sleep(300);
		
	}
	
	std::cin.get();
	return 0;
}