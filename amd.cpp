#include "application.h"

void sighdl(int sig) {

}

int main(int argc, char ** argv) {
	Application app(argc, argv);
	app.Run();
}


