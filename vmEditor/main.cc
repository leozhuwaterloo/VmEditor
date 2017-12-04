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


	unique_ptr<Window> window = make_unique<Window>(make_unique<KeyListener>(),
		make_unique<ColorManager>(),
		make_unique<DefaultParser>(),
		make_unique<DefaultSaver>());
	initCommands(window->getKeyListener());
	initColors(window->getColorManager());

	window->init(fileName);
	return 0;
}
