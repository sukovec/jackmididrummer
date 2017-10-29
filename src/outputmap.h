#pragma once

#include <vector>
#include <map>

#include "log.h"
#include "types.h"

class OutputMap {
	public:
		OutputMap();
		~OutputMap();
		void Initialize(std::vector<std::string> & outputs);
		outputref_t GetOut(std::string name);
		const char ** GetOutputList(int & size);
	private:
		std::map<std::string, outputref_t> map;
		const char ** outputs;
		int count;
};
