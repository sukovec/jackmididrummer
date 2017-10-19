#pragma once

#include <stdexcept>
#include <jack/jack.h>

#include "log.h"

class Jacker { 
	public:
	       	Jacker();
		~Jacker();
		void Open();
		void Close();
		
	private:
		jack_client_t * jackcl;
	
};
