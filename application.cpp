#include "application.h"

Application::Application(int argc, char ** argv) {
	log("Application::Application");
}

Application::~Application() {
	log("Application::~Application");
}

void Application::Run() {
	log("Application::Run");
	this->cfg.Open("config");
}


/*
jack_client_t * initjack() {
	jack_client_t * ret = jack_client_open("amd", JackNullOption, NULL);
	return ret;
}

void closejack(jack_client_t * cl) {
	jack_client_close(cl);
}



//	jack_client_t * cl = initjack();
	if (cl == NULL) {
		printf("creation of jack client failed\n");
		return 1;
	}
	printf("jack running\n");

	sleep(10000);
	closejack(cl);
*/
