#include <string>
#include "vm.h"
using namespace std;

int main(int argc, char **argv){
	int i;
	string fileName;

	//we will ignore any flags for now
	for(i = 1; i < argc; ++i){
		if(argv[i][0] != '-'){
			fileName = string(argv[i]);
			break;
		}
	}


	unique_ptr<Window> window = make_unique<Window>(fileName, make_unique<KeyListener>(), make_unique<ColorManager>());
	initCommands(window->getKeyListener());
	initColors(window->getColorManager());

	window->init();
	return 0;
}
