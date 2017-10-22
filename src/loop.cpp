#include "loop.h"

DrumLoop::DrumLoop(int bars, int div, Beat * beats) {
	log("DrumLoop::DrumLoop(%d, %d)", bars, div);
}

DrumLoop::~DrumLoop() {
	log("DrumLoop::~DrumLoop()");
}
