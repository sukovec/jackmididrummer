#pragma once

#include <vector>
#include <string>

namespace MIDI {
	enum class EventType : unsigned char{ // should be called MessageType
		NoteOn = 0b1001,
		ProgramChange = 0b1100,
		ControlChange = 0b1011, 
		Unknown = 0b11111111
	};

	struct Message { 
		EventType evttype;
		unsigned char channel;
		unsigned char notecc;
		unsigned char velocity;

		Message();
		Message(EventType type, unsigned char channel, unsigned char notecc, unsigned char velocity);
		int Encode(void * buf, int bufsize);
		bool IsValid() { return channel != 255; }
		static Message FromBuffer(void * buffer, int size); 
	};

}

typedef int noteref_t;
typedef int loopref_t;

struct Beat {
	int notecount;
	noteref_t * notes;
};

enum class ReactType {
	ChangeLoop, 
	ChangeTempo,
	ChangeChannel,
	StopDrumming,
	DoNothing // in case of error or whatever
};
struct Reaction {
	ReactType react;
	bool set;
	union {
		loopref_t chloop;
		int tempo; 	//
		int channel;
	};
};


struct CfgSendEvent {
	std::string name;
	MIDI::EventType type; // type of event to send
	int notecc; // note or CC/PC number
};

struct CfgMapping {
	std::string loopname;
	int argument1; // right now, this can be channel or tempo 
	MIDI::EventType type; // type of event to react
	int notecc; // note or CC/PC number
};

struct CfgLoop {
	std::string name;
	int barbeats;
	int bars;
	std::vector<std::vector<std::string>> beats; // a beat contains vector of notes
};

struct Configuration {
	int tempo;
	int channel;
	std::vector<CfgSendEvent> events;
	std::vector<CfgMapping> mapping;
	std::vector<CfgLoop> loops;
};
