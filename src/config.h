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
		void SetJackClientName(std::vector<std::string> tokens);
		void SetIO(std::string type, std::string line);

		MIDI::EventType StrToEventType(std::string str);

		// TODO: Make it into a struct
		std::vector<CfgSendEvent> sendevts;
		std::vector<CfgMappings> mappings;
		std::vector<CfgLoop> loops;
		std::vector<std::string> inputs;
		std::vector<std::string> outputs;
		std::string jackclname;

		/// members
		int cline;
		bool opened;

		/// config:
		int channel;
		int tempo;
};
