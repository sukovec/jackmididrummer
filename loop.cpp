#include "loop.h"

DrumLoop::DrumLoop(int bars, int div) {
	log("DrumLoop::DrumLoop(%d, %d)", bars, div);
}

DrumLoop::~DrumLoop() {
	log("DrumLoop::~DrumLoop()");
}
