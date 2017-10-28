#include "types.h"

namespace MIDI {
	Message::Message() {
		this->evttype = EventType::Unknown;
		this->channel = 255;
		this->velocity = 0;
		this->notecc = 0;
	}
	Message::Message(EventType type, unsigned char channel, unsigned char notecc, unsigned char velocity) {
		this->evttype = type;
		this->channel = channel;
		this->notecc = notecc;
		this->velocity = velocity;
	}

	Message Message::FromBuffer(void * buffer, int size) {
		Message ret;
		unsigned char * buf = (unsigned char *) buffer;

		// status byte: xxxxCCCC: xxxx - type (cc 1011, pc 1100, 1001 note on), CCCC - channel
		unsigned char type = (buf[0] & 0xF0) >> 4;
		unsigned char chnl = buf[0] & 0x0F; 

		ret.evttype = (EventType)type;
		switch(type) {
			case (unsigned char)EventType::ProgramChange:
				if (size != 2) return Message(EventType::Unknown, 255, 0, 0);
				break;

			case (unsigned char)EventType::ControlChange:
				if (size != 3) return Message(EventType::Unknown, 255, 0, 0);

				ret.velocity = buf[2]; // not a velocity, more like value
				break;

			case (unsigned char)EventType::NoteOn:
			case (unsigned char)EventType::NoteOff:
				if (size != 3) return Message(EventType::Unknown, 255, 0, 0);

				ret.velocity = buf[2];
				break;

			default:
				printf("eror\n");
				return Message(EventType::Unknown, 255, 0, 0);
		}

		ret.channel = chnl;
		ret.notecc = buf[1];

		return ret;
	}

	int Message::Encode(void * buffer, int maxsize) {
		int sz = 2;
		if (this->evttype == EventType::ControlChange || this->evttype == EventType::NoteOn)
			sz = 3;

		if (this->evttype == EventType::Unknown || sz > maxsize) {
			printf("Message::CreateBuffer: Buffer is too low to fit MIDI message (or message is of unknown type?) (%d)");
			return -1;
		}

		unsigned char * buf = (unsigned char *)buffer;
		buf[0] = (((unsigned char)this->evttype) & 0xF) << 4 | (this->channel & 0xF);
		buf[1] = this->notecc & 0x3F;
		if (sz == 3)
			buf[2] = this->velocity;

		return sz;
	}

	void Message::Print() {
		switch(this->evttype) {
			case EventType::ControlChange:
				printf("MIDI channel: %d ControlChange %d value %d\n", this->channel, this->notecc, this->velocity);
				break;
			case EventType::NoteOn:
				printf("MIDI channel: %d NoteOn %d velocity %d\n", this->channel, this->notecc, this->velocity);
				break;
			case EventType::NoteOff:
				printf("MIDI channel: %d NoteOff %d velocity %d\n", this->channel, this->notecc, this->velocity);
				break;
			case EventType::ProgramChange:
				printf("MIDI channel: %d ProgramChange %d\n", this->channel, this->notecc);
				break;
			default:
				printf("MIDI unknown message\n");
				break;
		}
	}
}
