#include <iostream>

#include "path_finder.h"


md::PathFinderApp pathFidnerApp;

int main(int argc, char *argv[])
{
	pathFidnerApp.Start();

	pathFidnerApp.GameLoop();

	pathFidnerApp.Close();

	return 0;
}