#pragma once
#include <vector>
#include <string>
#include <map>

#include "config.h"
#include "types.h"

class NoteMap {
	public:
		NoteMap(std::vector<CfgSendEvent> notes);
		~NoteMap();
		noteref_t GetNoteMapping(std::string note);
		inline CfgSendEvent const & operator[] (noteref_t x);
	private:
		CfgSendEvent * evts;
		std::map<std::string, noteref_t> nmap;

};
