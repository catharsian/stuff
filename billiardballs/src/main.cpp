
#include "endEngine.h"







int main()
{
	parser::initialize();
	parser::parse("./conf.cfg");
	baseEngine::initialize();
	endEngine game;
	game.Start();

	return 0;
}