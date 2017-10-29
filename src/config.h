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
		void ProcessLoopNormal(std::vector<std::string> tokens);
		void ProcessMultiloop(std::vector<std::string> tokens);
		void ProcessMapping(std::vector<std::string> tokens);
		void SetChannel(std::vector<std::string> tokens);
		void SetTempo(std::vector<std::string> tokens);
		void SetJackClientName(std::vector<std::string> tokens);
		void SetIO(std::vector<std::string> tokens, std::string line);
		void CreateOutput(std::vector<std::string> tokens);
		void SetDefault(std::vector<std::string> tokens);

		void ThrowError(const char * err);

		MIDI::EventType StrToEventType(std::string str);

		// TODO: Make it into a struct
		std::vector<CfgSendEvent> sendevts;
		std::vector<CfgMappings> mappings;
		std::vector<CfgLoop> loops;
		std::vector<std::string> inputconn;
		std::vector<CfgOutputConnection> outputconn;
		std::vector<std::string> outputs;

		std::string jackclname;
		std::string defaultloop;
		std::string defaultoutput;

		/// members
		int cline;
		bool opened;

		/// config:
		int channel;
		int tempo;
};
