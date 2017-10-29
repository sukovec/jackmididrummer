#pragma once
#include <vector>
#include <string>
#include <map>

#include "log.h"
#include "types.h"
#include "outputmap.h"

class NoteMap {
	public:
		NoteMap();
		~NoteMap();
		void Initialize(std::vector<CfgSendEvent> notes, OutputMap & map);
		noteref_t GetNoteMapping(std::string note);
		SendEvent const & operator[] (noteref_t x);
		void Stats();
	private:
		SendEvent * evts;
		std::map<std::string, noteref_t> nmap;

};
