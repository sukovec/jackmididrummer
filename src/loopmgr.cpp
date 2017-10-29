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
		printf("Error while resolving loop '%s'\n", loopname.c_str());
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

	switch(loop.type) {
		case CfgLoopType::Normal:
			this->NormalLoop(loop, map, loopidx);
			break;
		case CfgLoopType::Merge:
			this->LoopMerge(loop, loopidx);
			break;
		case CfgLoopType::Sequence:
			this->LoopSequence(loop, loopidx);
			break;
		default:
			printf("This is unhappenable code I hope\n");
			break;
	}
	
	this->loopmap.insert(std::pair<std::string, loopref_t>(loop.name, loopidx));
}
void LoopManager::NormalLoop(CfgLoop & loop, NoteMap & map, int loopidx) {
	log("LoopManager::NormalLoop()");

	int bc = loop.beats.size();
	Beat * beats = new Beat[bc];
	for (int i = 0; i < loop.beats.size(); i++) {
		beats[i].notecount = loop.beats[i].size();
		beats[i].notes = new noteref_t[beats[i].notecount];

		for (int j = 0; j < loop.beats[i].size(); j++) {
			beats[i].notes[j] = map.GetNoteMapping(loop.beats[i][j]);
		}
	}

	this->loops[loopidx].Initialize(bc, loop.bars, beats);
}

void LoopManager::LoopMerge(CfgLoop & loop, int loopidx) {
	log("LoopManager::LoopMerge()");

	int lpcnt = loop.beats[0].size();
	std::vector<DrumLoop*> loops;
	for (int i = 0; i < lpcnt; i++) {
		loops.push_back(this->GetLoop(this->GetLoopMapping(loop.beats[0][i])));
	}

	int lcm = this->LCMTempo(loops);

	int bc = 0; // beat count
	int div[lpcnt]; // every div[loopindex] is loop beat
	for (int i = 0; i < lpcnt; i++) {
		div[i] = lcm / loops[i]->GetBarBeats();
		bc = MAX(bc, loops[i]->GetTotalBeats() * div[i]);
	}

	Beat * beats = new Beat[bc];

	for (int i = 0; i < bc; i++) { // create beats
		std::vector<noteref_t> notes;

		for(int lp = 0; lp < lpcnt; lp++) {
			if (i % div[lp] == 0) {
				int beatnum = i / div[lp];
				if (loops[lp]->GetTotalBeats() <= beatnum) continue;
				Beat * cbeat = loops[lp]->GetBeat(beatnum);
				notes.insert(notes.end(), &cbeat->notes[0], &cbeat->notes[cbeat->notecount]);
			}
		}

		beats[i].notecount = notes.size();
		if (beats[i].notecount > 0)
			beats[i].notes = new noteref_t[notes.size()];
		else 
			beats[i].notes = nullptr;

		std::copy(notes.begin(), notes.end(), beats[i].notes);
	}

	this->loops[loopidx].Initialize(bc, lcm, beats);
}

void LoopManager::LoopSequence(CfgLoop & loop, int loopidx) {
	log("LoopManager::LoopSequence()");

	int lpcnt = loop.beats[0].size();
	std::vector<DrumLoop*> loops;
	for (int i = 0; i < lpcnt; i++) {
		loops.push_back(this->GetLoop(this->GetLoopMapping(loop.beats[0][i])));
	}

	int lcm = this->LCMTempo(loops);

	int bc = 0; // beat count
	int div[lpcnt]; // every div[loopindex] is loop beat
	for (int i = 0; i < lpcnt; i++) {
		div[i] = lcm / loops[i]->GetBarBeats();
		bc += loops[i]->GetTotalBeats() * div[i];
	}

	Beat * beats = new Beat[bc];

	int pos = 0;
	for (int lp = 0; lp < lpcnt; lp++) {
		for (int i = 0; i < loops[lp]->GetTotalBeats(); i++) {
			Beat * cbt = loops[lp]->GetBeat(i);
			beats[pos].notecount = cbt->notecount;
			beats[pos].notes = new noteref_t[cbt->notecount];
			memcpy(beats[pos].notes, cbt->notes, sizeof(cbt->notes[0]) * cbt->notecount);

			pos += div[lp];
		}
	}

	this->loops[loopidx].Initialize(bc, lcm, beats);
}

int LoopManager::LCMTempo(std::vector<DrumLoop*> cfl) {
	if (cfl.size() == 0) return 0;
	if (cfl.size() == 1) return cfl[0]->GetBarBeats();
	
	int ret = (cfl[0]->GetBarBeats() * cfl[1]->GetBarBeats()) / std::__gcd(cfl[0]->GetBarBeats(), cfl[1]->GetBarBeats());

	for (int i = 2; i < cfl.size(); i++)
		ret = (ret * cfl[i]->GetBarBeats()) / std::__gcd(ret, cfl[i]->GetBarBeats());

	return ret;
}
