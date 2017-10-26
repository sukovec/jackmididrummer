#include "reactmgr.h"


ReactManager::ReactManager() {
	log("ReactManager::ReactManager()");

	this->midinote = new Reaction[128];
	this->midicc = new Reaction[128];
	this->midipc = new Reaction[128];

	// nullize all reactions
	for (int i = 0; i < MAX_NOTE_VALUE; i++)
		this->midinote[i].count = this->midicc[i].count = this->midipc[i].count = 0;
}

ReactManager::~ReactManager() {
	log("ReactManager::~ReactManager()");

	for (int i = 0; i < MAX_NOTE_VALUE; i++) {
		if (this->midinote[i].count != 0)
			delete [] this->midinote[i].commands;
		if (this->midicc[i].count != 0)
			delete [] this->midicc[i].commands;
		if (this->midipc[i].count != 0)
			delete [] this->midipc[i].commands;
	}

	delete [] this->midinote;
	delete [] this->midicc;
	delete [] this->midipc;
}

void ReactManager::Initialize(std::vector<CfgMappings> & mapping, LoopManager & loopmgr) {
	log("ReactManager::Initialize()");
	for (int i = 0; i < mapping.size(); i++) {
		Reaction * reactset;
		switch (mapping[i].type) {
			case MIDI::EventType::NoteOn:
				reactset = this->midinote;
				break;
			case MIDI::EventType::ProgramChange:
				reactset = this->midipc;
				break;
			case MIDI::EventType::ControlChange:
				reactset = this->midicc;
				break;
			default:
				// this should not be user-inducible
				throw std::invalid_argument("Unknown kind of midi message type");
				break;
		}

		Reaction * react = &reactset[mapping[i].notecc];
		
		react->count = mapping[i].commands.size();
		react->commands = new Command[react->count];
		for (int j = 0; j < react->count; j++) {
			react->commands[j].command = mapping[i].commands[j].command;
			switch (react->commands[j].command) {
				case ReactType::ChangeLoop:
					react->commands[j].chloop = loopmgr.GetLoopMapping(mapping[i].commands[j].loopname);
					break;
				case ReactType::ChangeTempo:
					react->commands[j].tempo = mapping[i].commands[j].argument1;
					break;
				case ReactType::ChangeChannel:
					react->commands[j].channel = mapping[i].commands[j].argument1;
					break;
				case ReactType::StopDrumming:
				case ReactType::TapTempo:
					// nothing to do (yet?)
					break;
				default:
					throw std::invalid_argument("Unknown command");
					break;
			}
		}
	}
}

void ReactManager::Stats() {
	int count = 0;
	for (int i = 0; i < MAX_NOTE_VALUE; i++) {
		if (this->midinote[i].count) {
			count++;
		}
		if (this->midicc[i].count)
			count++;
		if (this->midipc[i].count) {
			count++;
		}
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
