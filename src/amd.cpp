#include "application.h"

void sighdl(int sig) {

}

int main(int argc, char ** argv) {
	Application app(argc, argv);
	try {
		app.Run();
	}
	catch (std::exception const & ex) {
		printf("\033[31mError:\033[0m %s\n", ex.what());
	}
}


