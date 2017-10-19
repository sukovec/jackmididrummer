#include "jacker.h"

Jacker::Jacker() {
	log("Jacker::Jacker()");

	this->jackcl = nullptr;
}

Jacker::~Jacker() {
	log("Jacker::~Jacker()");
	if (this->jackcl != nullptr)
		this->Close();
}

void Jacker::Open() {
	log("Jacker::Open()");
	this->jackcl = jack_client_open("amd", JackNullOption, NULL);

	if (this->jackcl == nullptr) 
		throw std::invalid_argument("Cannot initialize jack interface");

	log("Jack opened successfully");
}

void Jacker::Close() {
	log("Jacker::Close()");

	jack_client_close(this->jackcl);
}
