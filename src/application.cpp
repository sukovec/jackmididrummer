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
	log("=================================================");
	// make arrays from std::vector and create numeric mapping
	this->outputs.Initialize(cfg.outputs);
	this->map.Initialize(cfg.events, this->outputs);
	this->loops.Initialize(cfg.loops, this->map);
	this->reactions.Initialize(cfg.mapping, this->loops);

	/// just print statistics
	this->map.Stats();
	this->loops.Stats();
	this->reactions.Stats();

	// try to connect/create to jack server
	int outcnt;
	const char ** outs = this->outputs.GetOutputList(outcnt);
	this->jack.Open(cfg.jackclname.c_str(), outs, outcnt);
	
	// initialize drumming class
	this->drummer.Initialize(loops, map);
	this->drummer.SetTempo(cfg.tempo);
	this->drummer.SetEmitChannel(cfg.channel);
	this->drummer.UpdateParams(this->jack.GetSampleRate(), this->jack.GetBufferSize());

	this->jack.SetCallback(MessageCallback::from_function<Application, &Application::JackerCallback>(this));
	this->jack.SetGenerator(GeneratorCallback::from_function<Drummer, &Drummer::Drum>(&this->drummer));
	this->jack.Run();

	for (int i = 0; i < cfg.inputconn.size(); i++)
		this->jack.ConnectPorts(cfg.inputconn[i].c_str(), JackPortType::Input);


	for (int i = 0; i < cfg.outputconn.size(); i++)
		this->jack.ConnectPorts(cfg.outputconn[i].connect.c_str(), JackPortType::Output, this->outputs.GetOut(cfg.outputconn[i].output));
	
	sleep(-1);

	this->Close();
}

void Application::Close() {
	log("Application::Close()");
	this->jack.Close();
}

void Application::DoCommand(Command & cmd, jack_nframes_t time) {
	switch(cmd.command) {
		case ReactType::ChangeLoop:
			this->drummer.ChangeLoop(cmd.chloop);
			break;
		case ReactType::ChangeChannel:
			this->drummer.SetEmitChannel(cmd.channel);
			break;
		case ReactType::ChangeTempo:
			this->drummer.SetTempo(cmd.tempo);
			break;
		case ReactType::StopDrumming:
			this->drummer.StopDrumming();
			break;
		case ReactType::TapTempo:
			this->drummer.TapTempo(time);
			break;
	}
}

void Application::JackerCallback(MIDI::Message msg, jack_nframes_t time) {
	Reaction react = this->reactions.GetReaction(msg);
	if (react.count == 0) {
		printf("Received unmapped MIDI message: ");
		msg.Print();
	       	return;
	}

	for (int i = 0; i < react.count; i++) {
		this->DoCommand(react.commands[i], time);
	}
}
