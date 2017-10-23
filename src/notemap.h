#pragma once
#include <vector>
#include <string>
#include <map>

#include "log.h"
#include "types.h"

class NoteMap {
	public:
		NoteMap();
		~NoteMap();
		void Initialize(std::vector<CfgSendEvent> notes);
		noteref_t GetNoteMapping(std::string note);
		CfgSendEvent const & operator[] (noteref_t x);
		void Stats();
	private:
		CfgSendEvent * evts;
		std::map<std::string, noteref_t> nmap;

};
