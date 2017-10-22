#include "loop.h"

DrumLoop::DrumLoop() {
	log("DrumLoop::DrumLoop()");

	this->initialized = false;
}

DrumLoop::~DrumLoop() {
	log("DrumLoop::~DrumLoop()");

	if (this->initialized) {
		// deallocate beats
		for (int i = 0; i < this->beatcount; i++)
			delete [] this->beats[i].notes;
	}

	delete [] this->beats;
}

void DrumLoop::Initialize(int beatcount, int div, Beat * beats) {
	log("DrumLoop::Initialize(%d, %d)", beatcount, div);

	this->beats = beats;
	this->beatcount = beatcount;
	this->div = div;
}

inline int DrumLoop::GetBarBeats() {
	log("Drummer::GetBarBeats()");

	return this->div;
}

inline int DrumLoop::GetTotalBeats() {
	log("Drummer::GetTotalBeats()");

	return this->beatcount;
}

inline Beat * DrumLoop::GetBeat(int beat) {
	log("Drummer::GetBeat()");

	return &this->beats[beat];
}

