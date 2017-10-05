#pragma once

#ifndef MIDI_H
#define MIDI_H

namespace MIDI {
		enum class EventType {
			NoteOn, 
			ProgramChange,
			ControlChange
		};
}

#endif 
