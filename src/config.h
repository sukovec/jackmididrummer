#pragma once
//src: config.cpp

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>
#include <cctype>
#include <algorithm>


#include "log.h"
#include "midi.h"

struct SendEvent {
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
};

class Config {
	public:
		Config();
		~Config();
		void Open(const char * file);
		Configuration GetConfiguration();
	private:
		void ProcessNote(std::vector<std::string> tokens);
		void ProcessLoop(std::vector<std::string> tokens);
		void ProcessMapping(std::vector<std::string> tokens);
		void SetChannel(std::vector<std::string> tokens);

		MIDI::EventType StrToEventType(std::string str);

		std::vector<SendEvent> sendevts;
		std::vector<Mapping> mappings;
		std::vector<Loop> loops;

		/// members

		bool opened;

		/// config:
		int channel;
};
