#include "loopmgr.h"

LoopManager::LoopManager() {
	log("LoopManager::LoopManager()");
}

LoopManager::~LoopManager() {
	log("LoopManager::~LoopManager()");

	delete [] this->loops;
}

void LoopManager::Initialize(std::vector<CfgLoop> & loops, NoteMap & map) {
	log("LoopManager::Initialize()");

	this->loopcount = loops.size();
	this->loops = new DrumLoop[this->loopcount];
	for (int i = 0; i < this->loopcount; i++)
		this->ProcessLoop(loops[i], map, i);
}

loopref_t LoopManager::GetLoopMapping(std::string loopname) {
	if (this->loopmap.count(loopname) == 0) { // key doesn't exists
		throw std::invalid_argument("Loop doesn't exists");
	}

	return this->loopmap[loopname];
}

DrumLoop * LoopManager::GetLoop (loopref_t x) {
	log("LoopManager::operator [%d]", x);

	return &this->loops[x];
}

void LoopManager::Stats() {
	printf("LoopManager: Have %d loops\n", this->loopcount);
}

// private:
void LoopManager::ProcessLoop(CfgLoop & loop, NoteMap & map, int loopidx) {
	log("LoopManager::ProcessLoop()");

	int bc = loop.beats.size();
	Beat * beats = new Beat[bc];
	for (int i = 0; i < loop.beats.size(); i++) {
		beats[i].notecount = loop.beats[i].size();
		beats[i].notes = new noteref_t[beats[i].notecount];

		for (int j = 0; j < loop.beats[i].size(); j++) {
			beats[i].notes[j] = map.GetNoteMapping(loop.beats[i][j]);
		}
	}

	this->loops[loopidx].Initialize(bc, loop.barbeats, beats);
	this->loopmap.insert(std::pair<std::string, loopref_t>(loop.name, loopidx));
}
