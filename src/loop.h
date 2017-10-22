#pragma once

#include <vector>
#include <string>

#include "log.h"

class DrumLoop {
	public:
	       	DrumLoop(int beats, int div, Beat * beats);
		~DrumLoop();

	private:
		Beat * beats;
		int beats;
		int div;
};
