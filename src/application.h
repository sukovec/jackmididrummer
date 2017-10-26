// no reason to have a typical #ifndef - #define, this should not be included anywhere
//
// src: application.cpp

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <jack/jack.h>
#include <jack/midiport.h>

#include <map>

#include "log.h"
#include "config.h"
#include "loop.h"
#include "jacker.h"
#include "drummer.h"
#include "notemap.h"
#include "loopmgr.h"
#include "reactmgr.h"
#include "delegate.h"

class Application {
	public:
		Application();
		~Application();
		void Run(int argc, char ** argv);
		void Signal(int sig);
		void Close();

	private:
		void DoCommand(Command & cmd);
		void JackerCallback(MIDI::Message msg);

	private:
		Config cfg;
		Jacker jack;
		Drummer drummer;

		NoteMap map;
		LoopManager loops;
		ReactManager reactions;

};
