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
#include "types.h"

struct CfgSendEvent {
	std::string name;
	MIDI::EventType type; // type of event to send
	int notecc; // note or CC/PC number
};

struct CfgMapping {
	std::string loopname;
	MIDI::EventType type; // type of event to react
	int notecc; // note or CC/PC number
};

struct CfgLoop {
	std::string name;
	int barbeats;
	int bars;
	std::vector<std::vector<std::string>> beats; // a beat contains vector of notes
};

struct Configuration {
	int tempo;
	int channel;
	std::vector<CfgSendEvent> events;
	std::vector<CfgMapping> mapping;
	std::vector<CfgLoop> loops;
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
		void SetTempo(std::vector<std::string> tokens);

		MIDI::EventType StrToEventType(std::string str);

		std::vector<CfgSendEvent> sendevts;
		std::vector<CfgMapping> mappings;
		std::vector<CfgLoop> loops;

		/// members

		bool opened;

		/// config:
		int channel;
		int tempo;
};
