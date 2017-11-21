#include "application.h"

Application app;

void sighdl(int sig) {
	app.Close();
	exit(0);
}

int main(int argc, char ** argv) {
	signal(SIGINT, sighdl);

	try {
		app.Run(argc, argv);
	}
	catch (std::exception const & ex) {
		printf("\033[31mError:\033[0m %s\n", ex.what());
	}
}


