#include "notemap.h"

NoteMap::NoteMap() {
	log("NoteMap::NoteMap()");
}

noteref_t NoteMap::GetNoteMapping(std::string note) {
	if (this->nmap.count(note) == 0) { // key doesn't exists
		printf("Get mapping for note '%s'\n", note.c_str());
		throw std::invalid_argument("Note mapping doesn't exists");
	}

	return this->nmap[note];
}

void NoteMap::Initialize(std::vector<CfgSendEvent> notes) {
	log("NoteMap::Initialize()");
	this->evts = new CfgSendEvent[notes.size()];

	std::pair<std::map<std::string,int>::iterator,bool> ret;

	// make notes mapping 
	for (int i = 0; i < notes.size(); i++) {
		this->evts[i] = notes[i];
		ret = this->nmap.insert(std::pair<std::string, int>(this->evts[i].name, i));

		if (ret.second == false) {
			throw std::invalid_argument("Two \"notes\" have the same name, which is sooo wrong");
		}
	}
}

CfgSendEvent const & NoteMap::operator[] (noteref_t x) {
	return this->evts[x];
}

NoteMap::~NoteMap() {
	log("NoteMap::~NoteMap()");
	delete [] this->evts;
}

void NoteMap::Stats() {
	printf("NoteMap: Have %d notes mapped\n", this->nmap.size());
}
