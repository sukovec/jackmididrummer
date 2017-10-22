#include "loopmgr.h"

LoopManager::LoopManager(std::vector<CfgLoop> & loops, NoteMap & map) {
	log("LoopManager::LoopManager()");

	this->loops = new DrumLoop[loops.size()];
	for (int i = 0; i < loops.size(); i++)
		this->ProcessLoop(loops[i], map);


}

LoopManager::~LoopManager() {
	log("LoopManager::~LoopManager()");

	delete [] this->loops;
}

loop_t LoopManager::GetLoopMapping(std::string loopname) {

}

DrumLoop const & LoopManager::operator[] (loopref_t x) {
	log("LoopManager::operator [%d]", x);
}

// private:
void LoopManager::ProcessLoop(CfgLoop & loop, NoteMap & map) {
	log("LoopManager::ProcessLoop()");

	Beat beats[loop.beats.size()];
	for (int i = 0; i < loop.beats.size(); i++) {
		beats[i].notecount = loop.beats[i].size();
		beats[i].notes = new noteref_t[beats[i].notecount];

		for (int j = 0; j < loop.beats[i].size(); j++) {
			beats[i].notes[j] = map.GetNoteMapping(loop.beats[i][j]);
		}
	}



	/*struct Beat {
		int notes;
		noteref_t * emit;
	};*/
	/*
	   struct Loop {
	   std::string name;
	   int barbeats;
	   int bars;
	   std::vector<std::vector<std::string>> beats;
	   };*/

}
