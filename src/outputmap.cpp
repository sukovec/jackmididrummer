#include "outputmap.h"

OutputMap::OutputMap() {
	log("OutputMap::OutputMap()");

	this->outputs = nullptr;
}

OutputMap::~OutputMap() {
	log("OutputMap::OutputMap()");

	if (this->outputs != nullptr)
		delete [] this->outputs;
}

void OutputMap::Initialize(std::vector<std::string> & outputs) {
	log("OutputMap::Initialize(%d)", outputs.size());

	if (outputs.size() == 0) {
		this->outputs = new const char *[1];
		this->outputs[0] = "output";
		this->map.insert(std::pair<std::string, outputref_t>(this->outputs[0], 0));
		this->count = 1;
	} else {

		this->count = outputs.size();

		this->outputs = new const char*[this->count];
		std::pair<std::map<std::string,int>::iterator,bool> ret;

		for (int i = 0; i < outputs.size(); i++) {
			ret = this->map.insert(std::pair<std::string, outputref_t>(outputs[i], i));
			this->outputs[i] = outputs[i].c_str();

			if (ret.second == false) {
				throw std::invalid_argument("An output name must be original");
			}
		}
	}
	log("\tGot %d outputs", this->count);
}

outputref_t OutputMap::GetOut(std::string name) {
	if (name.empty()) {
		return 0; // default out
	}
	return this->map[name];
}

const char ** OutputMap::GetOutputList(int & size) {
	log("OutputMap::GetInputList()");

	size = this->count;
	return this->outputs;
}

