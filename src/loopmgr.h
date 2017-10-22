#pragma once

#include <string>
#include <vector>

#include "notemap.h"
#include "loop.h"

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

	private:
		int loopcount;
		DrumLoop * loops;
		std::map<std::string, loopref_t> loopmap;
};
