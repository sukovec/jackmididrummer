#pragma once

#include <stdexcept>
#include <jack/jack.h>
#include <jack/midiport.h>

#include "log.h"
#include "types.h"
#include "delegate.h"

class Jacker;
extern Jacker * c_jacker;

enum class JackPortType {
	Input, Output
};

class Jacker { 
	public:
	       	Jacker();
		~Jacker();
		void Open(const char * clname);
		void Close();
		void Run();

		// callbacks:
		int Process(jack_nframes_t nframes);
		void Shutdown();
		int BufferSizeChanged(jack_nframes_t size);
		void SetCallback(Delegate<void, MIDI::Message> callback);
		void SetGenerator(Delegate<void, Jacker *> callback);

		void SendMessage(MIDI::Message msg, int time);

		void ConnectPorts(const char * portname, JackPortType type);

		int GetBufferSize();
		int GetSampleRate();
	private:
		void CreatePorts();
		void ProcessMessage(jack_midi_event_t & evt);


		jack_client_t * jackcl;

		jack_port_t * input;
		jack_port_t * output;

		jack_nframes_t samplerate;
		jack_nframes_t buffersize;

		void * outbuffer;

		Delegate<void, MIDI::Message> msgcallback;
		Delegate<void, Jacker *> generatorcallback;
	
};
