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
		inline int GetBarBeats();
		inline int GetTotalBeats();
		inline Beat * GetBeat(int beat);

	private:
		bool initialized;
		Beat * beats;
		int beatcount;
		int div;
};
