#pragma once

typedef int noteref_t;
typedef int loopref_t;

struct Beat {
	int notecount;
	noteref_t * notes;
};

namespace MIDI {
		enum class EventType {
			NoteOn, 
			ProgramChange,
			ControlChange
		};
}
