#pragma once

#include <vector>
#include <string>

#include "log.h"
#include "types.h"

class DrumLoop {
	public:
	       	DrumLoop();
		~DrumLoop();
		void Initialize(int beatcount, int div, Beat * beats);
		int GetBarBeats();
		int GetTotalBeats();
		Beat * GetBeat(int beat);

	private:
		bool initialized;
		Beat * beats;
		int beatcount;
		int div;
};
