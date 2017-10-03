// no reason to have a typical #ifndef - #define, this should not be included anywhere
//
// src: application.cpp

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <jack/jack.h>
#include <jack/midiport.h>

#include "log.h"
#include "config.h"
#include "loop.h"



class Application {
	public:
		Application(int argc, char ** argv);
		~Application();
		void Run();
		void Signal(int sig);

	private:
		Config cfg;

};
