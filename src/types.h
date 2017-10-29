#pragma once

#include <vector>
#include <string>

#define DEFAULT_OUTPUT 0 // in index 0, i need to have default output all the time
#define DEFAULT_VELOCITY 127

namespace MIDI {
	enum class EventType : unsigned char{ // should be called MessageType
		NoteOn = 0b1001,
		NoteOff = 0b1000,
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
		void Print();
	};

}

typedef int noteref_t;
typedef int loopref_t;
typedef int outputref_t;

struct Beat {
	int notecount;
	noteref_t * notes;
};

enum class ReactType {
	ChangeLoop, 
	ChangeTempo,
	ChangeChannel,
	StopDrumming,
	TapTempo,
	DoNothing // in case of error or whatever
};

struct Command {
	ReactType command;
	union {
		loopref_t chloop;
		int tempo; 	//
		int channel;
	};
};

struct Reaction {
	int count; 
	Command * commands;
};

struct CfgSendEvent {
	std::string name;
	std::string output;
	MIDI::EventType type; // type of event to send
	int notecc; // note or CC/PC number
	int velocity;
};

struct SendEvent {
	MIDI::EventType type;
	int notecc;
	int output;
	int velocity;
	/*int velocity;*/ // for future use, maybe?

	SendEvent() {}
	SendEvent(MIDI::EventType type, int notecc, int velocity, int output = 0) {
		this->type = type;
		this->notecc = notecc;
		this->output = output; 
		this->velocity = velocity;
	}
};

#define CMD_TEMPO "!TEMPO"
#define CMD_STOP "!STOP"
#define CMD_CHANNEL "!CHANNEL"
#define CMD_TAPTEMPO "!TAPTEMPO"

struct CfgCommand {
	ReactType command;
	std::string loopname;
	int argument1; // right now, this can be channel or tempo 
};
 
struct CfgMappings {
	MIDI::EventType type; // type of event to react
	int notecc; // note or CC/PC number

	std::vector<CfgCommand> commands;
};

enum class CfgLoopType { Normal, Merge, Sequence } ;

struct CfgLoop {
	std::string name;
	CfgLoopType type;

	int barbeats;
	int bars;
	std::vector<std::vector<std::string>> beats; 	// a beat contains vector of notes
							// when a multiloop or merge, then loops should go to 
							// first "beat"
};

struct CfgOutputConnection {
	std::string output;
	std::string connect;

	CfgOutputConnection(std::string out, std::string conn) { output = out; connect = conn; }
};

struct Configuration {
	int tempo;
	int channel;
	std::vector<CfgSendEvent> events;
	std::vector<CfgMappings> mapping;
	std::vector<CfgLoop> loops;
	std::vector<std::string> outputs;
	std::vector<std::string> inputconn;
	std::vector<CfgOutputConnection> outputconn;

	std::string jackclname;
	std::string defaultloop;
	std::string defaultoutput;
};
