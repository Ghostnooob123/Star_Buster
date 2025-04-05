// C.cpp : Defines the entry point for the application.
//

#include "C.h"

int main(int argc, char* argv[]) {

    Engine en = Engine();

	while (en.running())
	{
		en.update();
		en.render();
	}

    return 0;
}