#include "drummer.h"
Drummer::Drummer() {
	log("Drummer::Drummer");

	this->drumming = false;
	this->next = nullptr;
	this->current = nullptr;
}

Drummer::~Drummer() {
	log("Drummer::~Drummer");
}

void Drummer::Initialize(LoopManager & loops, NoteMap & notes) {
	log("Drummer::Initialize()");

	this->loops = &loops;
	this->notes = &notes;
}

void Drummer::ChangeLoop(loopref_t loop) {
	log("Drummer::ChangeLoop(%d)", loop);

	this->next = this->loops->GetLoop(loop);

	this->StartDrumming();
}

void Drummer::SetEmitChannel(int channel) {
	log("Drummer::SetEmitChannel(%d)", channel);
	
	this->nextchannel = channel;
}

void Drummer::SetTempo(int tempo) {
	log("Drummer::SetTempo(%d)", tempo);

	this->nexttempo = tempo;
}

void Drummer::StartDrumming() {
	log("Drummer::StartDrumming()");

	this->Proceed();
	this->curpos = 0;
	this->drumming = this->current != nullptr;
}

void Drummer::StopDrumming() {
	log("Drummer::StopDrumming()");

	this->next = nullptr;
}

void Drummer::UpdateParams(int splrate, int buffersize) {
	log("Drummer::UpdateParams(%d, %d)", splrate, buffersize);

	this->splrate = splrate;
	this->buffersize = buffersize;
}

void Drummer::Drum(Jacker * jack) {
	if (!this->drumming) return;

	if (this->curpos % 20480 == 0) {
		jack->SendMessage(MIDI::Message(MIDI::EventType::NoteOn, 1, 38, 127), 0);
	}

	if (this->curpos % 20480 == 0) {
		jack->SendMessage(MIDI::Message(MIDI::EventType::NoteOn, 1, 36, 127), 0);
	}

	if (this->curpos % 20480 == 0) {
		jack->SendMessage(MIDI::Message(MIDI::EventType::NoteOn, 1, 42, 127), 0);
	}

	if (this->curpos % 20480 == 0) {
		jack->SendMessage(MIDI::Message(MIDI::EventType::NoteOn, 1, 49, 127), 0);
	}

	this->curpos += this->buffersize;
}

void Drummer::Proceed() { 
	// there is no need of conditional - the next-values stays here until next change
	this->current = this->next;
	this->curtempo = this->nexttempo;
	this->curchannel = this->nextchannel;

	if (this->current == nullptr) {
		log("Drummer::Proceed: Next is set no nothing, so stopping");
		this->drumming = false;
	}
}
