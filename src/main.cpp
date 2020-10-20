#pragma once
#include <vector>
#include <iostream>
#include "Controller.h"

int main() try
{
	Controller game;
	game.run();
	return EXIT_SUCCESS;
}
 
catch (std::exception & e)
{
	std::cerr << "Exception: " << e.what() << '\n'; return EXIT_FAILURE;
}

catch (...)
{
	std::cerr << "Unknown exception\n"; return EXIT_FAILURE;
} 