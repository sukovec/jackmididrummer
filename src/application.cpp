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
	this->jack.Open();
	this->drummer.Prepare(&cfg);
}

void Application::MapConfiguration(Configuration & cfg) {
	// make array of notes (i.e. (A, B, C) and make map of indices

	std::map<std::string, int> notemapping;
	std::map<std::string, int> loopmapping;

	SendEvent * evarr = new SendEvent[cfg.events.size()];


	std::pair<std::map<std::string,int>::iterator,bool> ret;

	// make notes mapping 
	for (int i = 0; i < cfg.events.size(); i++) {
		evarr[i] = cfg.events[i];
		ret = notemapping.insert(std::pair<std::string, int>(evarr[i].name, i));

		if (ret.second == false) {
			throw std::invalid_argument("Two \"notes\" have the same name, which is sooo wrong");
		}
	}

	// load drum loops

	for (int i = 0; i < cfg.loops.size(); i++) {

	}



		/*struct SendEvent {
			std::string name;
			MIDI::EventType type; // type of event to send
			int notecc; // note or CC/PC number
		};

		struct Mapping {
			std::string loopname;
			MIDI::EventType type; // type of event to react
			int notecc; // note or CC/PC number
		};

		struct Loop {
			std::string name;
			int barnotes;
			int bars;
			std::vector<std::vector<std::string>> notes;
		};

		struct Configuration {
			int tempo;
			std::vector<SendEvent> events;
			std::vector<Mapping> mapping;
			std::vector<Loop> loops;
		};*/
}


