#pragma once

#include "log.h"
#include "config.h"
#include "jacker.h"

class Drummer { 
	public:
		Drummer();
		~Drummer();
		void Prepare(Configuration * cfg);
	private:
		Configuration * cfg;
};
