#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include <string.h>

#include "notemap.h"
#include "loop.h"

#define MAX(a,b) (((a) > (b)) ? (a) : (b))

class LoopManager { 
	public:
		LoopManager();
		~LoopManager();
		void Initialize(std::vector<CfgLoop> & loop, NoteMap & map);

		loopref_t GetLoopMapping(std::string loopname);
		DrumLoop * GetLoop (loopref_t x);
		void Stats();

	private:
		void ProcessLoop(CfgLoop & loop, NoteMap & map, int loopidx);
		void NormalLoop(CfgLoop & loop, NoteMap & map, int loopidx);
		void LoopMerge(CfgLoop & loop, int loopidx);
		void LoopSequence(CfgLoop & loop, int loopidx);
		int LCMTempo(std::vector<DrumLoop*> cfl);

	private:
		int loopcount;
		DrumLoop * loops;
		std::map<std::string, loopref_t> loopmap;
};
