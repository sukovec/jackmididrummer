#include "config.h"

Config::Config() {
	log("Config::Config()");
}

Config::~Config() {
	log("Config::~Config()");
}

void Config::Open(const char * fname) {
	log("Config::Open(%s)", fname);

	std::ifstream file(fname); // open file for reading

	std::string line;

	while (std::getline(file, line)) {
		if (line[0] == '#' || line.empty())
			continue;

		std::stringstream ss(line);
		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;
		std::vector<std::string> tokens(begin, end);

		if (tokens[1].compare("is") == 0) {
			this->ProcessNote(tokens);
		} else if (tokens[1].compare("=") == 0) {
			this->ProcessLoop(tokens);
		} else if (tokens[1].compare(":") == 0) {
			this->ProcessMapping(tokens);
		}
	}
}

void Config::ProcessNote(std::vector<std::string> tokens) {
	log("Config::ProcessNote()");
}

void Config::ProcessLoop(std::vector<std::string> tokens) {
	log("Config::ProcessLoop()");
}

void Config::ProcessMapping(std::vector<std::string> tokens) {
	log("Config::ProcessMapping()");
}

/*
# spaces between tokens are arbitrary ("MAP : PC 1" is OK, "MAP:PC 1" is not)
# use only one space
# everything is case sensitive

# notes

KICK is note 36 # C3
SR is note 38 # D3
HH is note 40 # E3

# loops

BUMC = 1x4 | KICK HH | HH | SR HH | HH | 
DNB  = 1x8 | HH KICK | HH | HH SR | HH | HH | HH KICK | HH SNARE | HH | 

# mapping of starting

BUMC : PC 1
BUMC : PC 2
DNB : PC 3
DNB : note 50
*/
