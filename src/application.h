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

class Application {
	public:
		Application(int argc, char ** argv);
		~Application();
		void Run();
		void Signal(int sig);

	private:
		void MapConfiguration(Configuration & cfg);

	private:
		Config cfg;
		Jacker jack;
		Drummer drummer;

};
