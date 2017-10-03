#pragma once
//src: config.cpp

#ifndef CONFIG_H
#define CONFIG_H

#include "log.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iterator>

class Config {
	public:
		Config();
		~Config();
		void Open(const char * file);
	private:
		void ProcessNote(std::vector<std::string> tokens);
		void ProcessLoop(std::vector<std::string> tokens);
		void ProcessMapping(std::vector<std::string> tokens);

		/// members

		bool opened;
};


#endif

