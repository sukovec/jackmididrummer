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

		if (tokens[0].compare("TEMPO") == 0) {
			this->SetTempo(tokens);
		} else if (tokens[0].compare("CHANNEL") == 0) {
			this->SetChannel(tokens);
		} else if (tokens[1].compare("is") == 0) {
			this->ProcessNote(tokens);
		} else if (tokens[1].compare("=") == 0) {
			this->ProcessLoop(tokens);
		} else if (tokens[2].compare(":") == 0) {
			this->ProcessMapping(tokens);
		}
	}
}

void Config::SetChannel(std::vector<std::string> tokens) {
	log("Config::SetChannel()");
	this->channel = std::stoi(tokens[1]);

	log("  Channel set to %d", this->channel);
}

void Config::SetTempo(std::vector<std::string> tokens) {
	log("Config::SetTempo()");
	this->tempo = std::stoi(tokens[1]);

	log("  Tempo set to %d", this->tempo);
}

void Config::ProcessNote(std::vector<std::string> tokens) {
	log("Config::ProcessNote()");

	CfgSendEvent sev;

	sev.name = tokens[0];
	sev.type = this->StrToEventType(tokens[2]);
	sev.notecc = std::stoi(tokens[3]);

	this->sendevts.push_back(sev);
}


void Config::ProcessLoop(std::vector<std::string> tokens) {
	log("Config::ProcessLoop()");

	CfgLoop loop;
	loop.name = tokens[0]; 
	loop.barbeats = std::stoi(tokens[2]);
	loop.bars = std::stoi(tokens[3]);
	
	if (tokens[4].compare("|") != 0) throw std::invalid_argument("Loop does not start with '|' character");

	log("Processing loop");
	int i = 5; // there should be 
	std::vector<std::string> notes;
	while (i < tokens.size()) {
		if (tokens[i].compare("|") == 0) { // 
			log("  found |");
			loop.beats.push_back(notes);
			notes.clear();
		} else if (tokens[i].compare("!") == 0) { // this will mark some space for configuration flags of loop
			log("  found !");
			break;
		}
		else {
			log("  found note");
			notes.push_back(tokens[i]);
		}

		i++;
	}

	this->loops.push_back(loop);
}

void Config::ProcessMapping(std::vector<std::string> tokens) {
	log("Config::ProcessMapping()");

	// proces mapping in form
	// MESSAGETYPE NUMBER : REACTION
	// CC 25 : DRUMLOOP
	// CC 26 : !TEMPO 66

	CfgMapping map;


	map.loopname = tokens[3];
	map.type = this->StrToEventType(tokens[0]);
	map.notecc = std::stoi(tokens[1]);

	if (tokens.size() == 5) {
		map.argument1 = std::stoi(tokens[4]);
	}

	this->mappings.push_back(map);
}

MIDI::EventType Config::StrToEventType(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	if (str.compare("note") == 0)
		return MIDI::EventType::NoteOn;
	else if (str.compare("pc") == 0)
		return MIDI::EventType::ProgramChange;
	else if (str.compare("cc") == 0)
		return MIDI::EventType::ControlChange;

	throw new std::invalid_argument("Invalid argument (must be 'note', 'pc' or 'cc')");
}

Configuration Config::GetConfiguration() {
	log("Config::GetConfiguration()");
	Configuration ret;
	ret.mapping = this->mappings;
	ret.loops = this->loops;
	ret.events = this->sendevts;

	ret.channel = this->channel;
	ret.tempo = this->tempo;

	return ret;
}

/*
# spaces between tokens are arbitrary ("MAP : PC 1" is OK, "MAP:PC 1" is not)
# use only one space
# everything is case sensitive except strings "note", "pc", "cc" in type

# notes/sounds/outputs

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
