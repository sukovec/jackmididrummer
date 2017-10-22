#include "reactmgr.h"


ReactManager::ReactManager() {
	log("ReactManager::ReactManager()");

	this->midinote = new Reaction[128];
	this->midicc = new Reaction[128];
	this->midipc = new Reaction[128];

	for (int i = 0; i < MAX_NOTE_VALUE; i++)
		this->midinote[i].set = this->midicc[i].set = this->midipc[i].set = false;
}

ReactManager::~ReactManager() {
	log("ReactManager::~ReactManager()");
	delete [] this->midinote;
	delete [] this->midicc;
	delete [] this->midipc;
}

void ReactManager::Initialize(std::vector<CfgMapping> & mapping, LoopManager & loopmgr) {
	log("ReactManager::Initialize()");
	for (int i = 0; i < mapping.size(); i++) {
		Reaction setreact;
		if (mapping[i].loopname.at(0) == '!') {
			if (mapping[i].loopname.compare("!TEMPO") == 0) {
				setreact.react = ReactType::ChangeTempo;
				setreact.tempo = mapping[i].argument1;
			} else if (mapping[i].loopname.compare("!CHANNEL") == 0) {
				setreact.react = ReactType::ChangeChannel;
				setreact.channel = mapping[i].argument1;
			} else if (mapping[i].loopname.compare("!STOP") == 0) {
				setreact.react = ReactType::StopDrumming;
			} else {
				throw std::invalid_argument("Unknown reaction directive (not loop, not !TEMPO, not !CHANNEL");
			}
		}
		else  {
			setreact.react = ReactType::ChangeLoop;
			setreact.chloop = loopmgr.GetLoopMapping(mapping[i].loopname);
		}
		setreact.set = true;

		Reaction * react;
		switch (mapping[i].type) {
			case MIDI::EventType::NoteOn:
				react = this->midinote;
				break;
			case MIDI::EventType::ProgramChange:
				react = this->midipc;
				break;
			case MIDI::EventType::ControlChange:
				react = this->midicc;
				break;
			default:
				throw std::invalid_argument("Unknown kind of midi message type");
				break;
		}

		react[mapping[i].notecc] = setreact;
	}
}

void ReactManager::Stats() {
	int count = 0;
	for (int i = 0; i < MAX_NOTE_VALUE; i++) {
		if (this->midinote[i].set)
			count++;
		if (this->midicc[i].set)
			count++;
		if (this->midipc[i].set)
			count++;
	}

	printf("Reactions: Have %d reactions\n", count);
}

Reaction & ReactManager::GetReaction(MIDI::Message msg) {
	Reaction * react;
	switch(msg.evttype) {
		case MIDI::EventType::NoteOn:
			react = this->midinote;
			break;
		case MIDI::EventType::ProgramChange:
			react = this->midipc;
			break;
		case MIDI::EventType::ControlChange:
			react = this->midicc;
			break;
		default:
			return this->empty;
			break;
	}

	return react[msg.notecc];
}
