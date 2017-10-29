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
	this->cline = 0;
	while (std::getline(file, line)) {
		this->cline++;
		if (line[0] == '#' || line.empty())
			continue;
		log("Processing line %d ('%s')", this->cline, line.c_str());

		std::stringstream ss(line);
		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;
		std::vector<std::string> tokens(begin, end);

		// TODO: Check token count
		// TODO: Use #defines for strings
		if (tokens[0].compare("TEMPO") == 0) {
			this->SetTempo(tokens);
		} else if (tokens[0].compare("CHANNEL") == 0) {
			this->SetChannel(tokens);
		} else if (tokens[0].compare("CONNECT") == 0) {
			this->SetIO(tokens, line);
		} else if (tokens[0].compare("CLIENT") == 0) {
			this->jackclname = tokens[1];
		} else if (tokens[0].compare("OUTPUT") == 0) {
			this->CreateOutput(tokens);
		} else if (tokens[0].compare("DEFAULT") == 0) {
			this->SetDefault(tokens);
		} else if (tokens[1].compare("is") == 0) {
			this->ProcessNote(tokens);
		} else if (tokens[1].compare("=") == 0) {
			this->ProcessLoop(tokens);
		} else if (tokens[2].compare(":") == 0) {
			this->ProcessMapping(tokens);
		}
	}
}

void Config::CreateOutput(std::vector<std::string> tokens) {
	log("Config::CreateOutput()");

	if (tokens.size() == 2) {
		this->outputs.push_back(tokens[1]);

		if (this->defaultoutput.empty())
			this->defaultoutput = tokens[1];
	}
	else {
		this->ThrowError("Too few/too much arguments (should be 'OUTPUT outputname' - no spaces etc)");
	}
}

void Config::SetDefault(std::vector<std::string> tokens) {
	log("Config::SetDefault()");

	if (tokens.size() < 3) {
		this->ThrowError("Too few/too much arguments");
	} else if (tokens[1].compare("OUTPUT") == 0) {

	} else if (tokens[1].compare("LOOP") == 0) {
		this->defaultloop = tokens[2];
	} else {
		this->ThrowError("Unknown directive (currently settable defaults are: OUTPUT, LOOP)");
	}
}
void Config::SetJackClientName(std::vector<std::string> tokens) {
	log("Config::SetJackClientName()");
	this->jackclname = tokens[1];
}

void Config::SetIO(std::vector<std::string> tokens, std::string line) {
	log("Config::SetIO()");
	// three possibilities:
	// CONNECT INPUT here is name of some jack output
	// CONNECT OUTPUT here is name of some jack input
	// CONNECT OUTPUT jmd_output > and here some jack input

	if (tokens.size() <= 2) {
		printf("When processing config line %d\n", this->cline);
		throw std::invalid_argument("Too few arguments for CONNECT");
	} 

	if (tokens[1].compare("INPUT") == 0) {
		this->inputconn.push_back(line.substr(tokens[0].length() + tokens[1].length() + 2));
	} else if (tokens[1].compare("OUTPUT") == 0) {
		int redirpos = -1;
		for (int i = 2; i < tokens.size(); i++) {
			if (tokens[i].compare(">") == 0) {
				redirpos = i;
				break;
			}
		}

		if (redirpos == -1) {  // it's not tied to non-default output
			this->outputconn.push_back(CfgOutputConnection("", line.substr(tokens[0].length() + tokens[1].length() + 2)));
		} else {
			this->outputconn.push_back(CfgOutputConnection(tokens[2], line.substr(tokens[0].length() + tokens[1].length() + tokens[2].length() + tokens[3].length() + 3)));
		}

	}
	else {
		printf("When processing config file line %d\n", this->cline);
		throw std::invalid_argument("Wrong directive (expected INPUT or OUTPUT)");
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

	if (tokens.size() < 4) 
		this->ThrowError("Too few or too much arguments for a send event");
	

	CfgSendEvent sev;

	sev.name = tokens[0];
	sev.type = this->StrToEventType(tokens[2]);
	sev.notecc = std::stoi(tokens[3]);
	sev.velocity = DEFAULT_VELOCITY;

	for (int i = 4; i < tokens.size(); i++) {
		if (tokens[i].compare("OUTPUT") == 0 && tokens.size() > i + 1) {
			sev.output = tokens[++i];
		} else if (tokens[i].compare("VELOCITY") == 0) {
			sev.velocity = std::stoi(tokens[++i]);
		} else if (tokens[i].at(0) == '#') {
			break;
		} else {
			this->ThrowError("Unknown token");
		}
	}

	this->sendevts.push_back(sev);
}


void Config::ProcessLoop(std::vector<std::string> tokens) {
	log("Config::ProcessLoop()");

	if (tokens[2].at(0) == '!') 
		this->ProcessMultiloop(tokens);
	else 
		this->ProcessLoopNormal(tokens);
}

void Config::ProcessLoopNormal(std::vector<std::string> tokens) {
	log("Config::ProcessLoopNormal()");

	CfgLoop loop;
	loop.name = tokens[0]; 
	loop.type = CfgLoopType::Normal;
	loop.barbeats = std::stoi(tokens[2]);
	loop.bars = std::stoi(tokens[3]);
	
	if (tokens[4].compare("|") != 0) throw std::invalid_argument("Loop does not start with '|' character");

	log("Processing loop");
	int i = 5; // there should be 
	std::vector<std::string> notes;
	while (i < tokens.size()) {
		if (tokens[i].compare("|") == 0) { // 
			loop.beats.push_back(notes);
			notes.clear();
		} else if (tokens[i].compare("!") == 0) { // this will mark some space for configuration flags of loop
			break;
		}
		else {
			notes.push_back(tokens[i]);
		}

		i++;
	}

	this->loops.push_back(loop);
}

void Config::ProcessMultiloop(std::vector<std::string> tokens) {
	log("Config::ProcessMultiloop()");

	CfgLoop loop;
	loop.name = tokens[0]; 
	// token[1] = "="
	// token[2] = "!MERGE"
	// token[3..n] = loopname
	
	if (tokens[2].compare("!MERGE") == 0)
		loop.type = CfgLoopType::Merge;
	else if (tokens[2].compare("!SEQUENCE") == 0)
		loop.type = CfgLoopType::Sequence;
	else 
		this->ThrowError("Unknown directive");

	loop.beats.push_back(std::vector<std::string>());
	
	std::vector<std::string> & tomerge = loop.beats[0];

	for (int i = 3; i < tokens.size(); i++) {
		if (tokens[i].at(0) == '#') break;

		tomerge.push_back(tokens[i]);
	}

	this->loops.push_back(loop);
}

void Config::ProcessMapping(std::vector<std::string> tokens) {
	log("Config::ProcessMapping()");

	// proces mapping in form
	// MESSAGETYPE NUMBER : REACTION1 REACTION2 REACTION3
	// CC 25 : DRUMLOOP
	// CC 26 : !TEMPO 66
	// PC 88 : DRUMLOOP !TEMPO 88

	CfgMappings map;
	map.type = this->StrToEventType(tokens[0]);
	map.notecc = std::stoi(tokens[1]);

	for (int i = 3; i < tokens.size(); ) {
		if (tokens[i].at(0) != '!') { // it is not a change loop comand
			map.commands.push_back({ ReactType::ChangeLoop, tokens[i], 0 });
			i++;
		} else if (tokens[i].compare(CMD_TEMPO) == 0)   {
			map.commands.push_back({ ReactType::ChangeTempo, std::string(), std::stoi(tokens[i+1]) });
			i += 2;
		} else if (tokens[i].compare(CMD_CHANNEL) == 0) {
			map.commands.push_back({ ReactType::ChangeTempo, std::string(), std::stoi(tokens[i+1]) });
			i += 2;
		} else if (tokens[i].compare(CMD_TAPTEMPO) == 0) {
			map.commands.push_back({ ReactType::TapTempo, std::string(), 0});
			i++;
		} else if (tokens[i].compare(CMD_STOP) == 0) {
			map.commands.push_back({ ReactType::StopDrumming, std::string(), 0});
			i++;
		} else {
			this->ThrowError("Unknown command");
		}
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

	this->ThrowError("Invalid midi message definition (must be 'note', 'pc', or 'cc')");
}

Configuration Config::GetConfiguration() {
	log("Config::GetConfiguration()");
	Configuration ret;
	ret.mapping = this->mappings;
	ret.loops = this->loops;
	ret.events = this->sendevts;

	ret.channel = this->channel;
	ret.tempo = this->tempo;

	ret.outputs = this->outputs;
	ret.inputconn = this->inputconn;
	ret.outputconn = this->outputconn;

	ret.jackclname = this->jackclname;
	ret.defaultloop = this->defaultloop;
	ret.defaultoutput = this->defaultoutput;

	return ret;
}

void Config::ThrowError(const char * err) {
	printf("While processing line %d\n", this->cline);
	throw std::invalid_argument(err);
}
