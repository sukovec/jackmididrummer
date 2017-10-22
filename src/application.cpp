#include "application.h"

Application::Application(int argc, char ** argv) {
	log("Application::Application()");
}

Application::~Application() {
	log("Application::~Application()");
}

void Application::Run() {
	log("Application::Run()");
	this->cfg.Open("config");

	Configuration cfg = this->cfg.GetConfiguration();

	NoteMap map(cfg.events);
	LoopManager loops(cfg.loops, map);

	this->jack.Open();
	this->drummer.Prepare(&cfg);
}

