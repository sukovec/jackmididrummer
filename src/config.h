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
		ReactType GetCommandType(std::string cmd, int & retargs);

		MIDI::EventType StrToEventType(std::string str);

		std::vector<CfgSendEvent> sendevts;
		std::vector<CfgMappings> mappings;
		std::vector<CfgLoop> loops;

		/// members
		int cline;
		bool opened;

		/// config:
		int channel;
		int tempo;
};
