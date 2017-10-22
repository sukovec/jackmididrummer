#pragma once

#include <string>
#include <vector>

#include "config.h"
#include "notemap.h"
#include "loop.h"

typedef int loop_t;

class LoopManager { 
	public:
		LoopManager(std::vector<CfgLoop> & loop, NoteMap & map);
		~LoopManager();

		loop_t GetLoopMapping(std::string loopname);
		inline DrumLoop const & operator[] (loop_t x);

	private:
		void ProcessLoop(CfgLoop & loop, NoteMap & map);

	private:
		DrumLoop * loops;
		std::map<std::string, loop_t> loopmap;
};
