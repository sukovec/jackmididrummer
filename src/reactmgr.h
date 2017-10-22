#pragma once

#include "types.h"
#include "loopmgr.h"

#define MAX_NOTE_VALUE 127 // PC, CC and Note can't have bigger number

// class that converts received midi messages to actions
class ReactManager {
	public:
		ReactManager();
		~ReactManager();
		void Initialize(std::vector<CfgMapping> & map, LoopManager & mgr);
		void Stats();

		Reaction & GetReaction(MIDI::Message msg);
	private:
		Reaction * midinote;
		Reaction * midicc;
		Reaction * midipc;

		Reaction empty { ReactType::DoNothing, false, 0 };
};
