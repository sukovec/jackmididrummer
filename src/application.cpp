#include "application.h"

Application::Application() {
	log("Application::Application()");
}

Application::~Application() {
	log("Application::~Application()");
}

void Application::Run(int argc, char ** argv) {
	log("Application::Run()");
	this->cfg.Open("config");

	// load configuration...
	Configuration cfg = this->cfg.GetConfiguration();

	// make arrays from std::vector and create numeric mapping
	this->map.Initialize(cfg.events);
	this->loops.Initialize(cfg.loops, this->map);
	this->reactions.Initialize(cfg.mapping, this->loops);

	/// just print statistics
	this->map.Stats();
	this->loops.Stats();
	this->reactions.Stats();

	// try to connect/create to jack server
	this->jack.Open();
	
	// initialize drumming class
	this->drummer.Initialize(loops, map);
	this->drummer.SetTempo(cfg.tempo);
	this->drummer.SetEmitChannel(cfg.channel);
	this->drummer.UpdateParams(this->jack.GetSampleRate(), this->jack.GetBufferSize());

	this->jack.SetCallback(Delegate<void, MIDI::Message>::from_function<Application, &Application::JackerCallback>(this));
	this->jack.SetGenerator(Delegate<void, Jacker *>::from_function<Drummer, &Drummer::Drum>(&this->drummer));
	this->jack.Run();

	sleep(-1);

	this->Close();
}

void Application::Close() {
	log("Application::Close()");
	this->jack.Close();
}

void Application::JackerCallback(MIDI::Message msg) {
	Reaction react = this->reactions.GetReaction(msg);
	if (!react.set) {
		log("Reaction: Do nothing");
	       	return;
	}

	switch(react.react) {
		case ReactType::ChangeLoop:
			this->drummer.ChangeLoop(react.chloop);
			break;
		case ReactType::ChangeChannel:
			this->drummer.SetEmitChannel(react.channel);
			break;
		case ReactType::ChangeTempo:
			this->drummer.SetTempo(react.tempo);
			break;
		case ReactType::StopDrumming:
			this->drummer.StopDrumming();
			break;
	}
}
