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

// TYPES:
typedef Delegate<void, MIDI::Message, jack_nframes_t> MessageCallback;
typedef Delegate<void, Jacker *> GeneratorCallback;

class Jacker { 
	public:
	       	Jacker();
		~Jacker();
		void Open(const char * clname, const char ** outputs, int outputcount);
		void Close();
		void Run();

		// callbacks:
		int Process(jack_nframes_t nframes);
		void Shutdown();
		int BufferSizeChanged(jack_nframes_t size);
		void SetCallback(MessageCallback callback);
		void SetGenerator(GeneratorCallback callback);

		void SendMessage(MIDI::Message msg, int time, outputref_t out);

		void ConnectPorts(const char * portname, JackPortType type, outputref_t out = 0);

		int GetBufferSize();
		int GetSampleRate();
	private:
		void CreatePorts(const char ** outputs, int outputcount);
		void ProcessMessage(jack_midi_event_t & evt);


		jack_client_t * jackcl;

		jack_port_t * input;
		jack_port_t ** outputs;

		jack_nframes_t samplerate;
		jack_nframes_t buffersize;

		void ** outbuffer;
		int outputcount;

		MessageCallback msgcallback;
		GeneratorCallback generatorcallback;
	
};
