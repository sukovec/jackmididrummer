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

	log("\tLoop beats: %d, div: %d", this->next->GetTotalBeats(), this->next->GetBarBeats());

	if (!this->drumming) this->StartDrumming();
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

	int div = this->current->GetBarBeats();

	int splperbeat = (this->splrate * 60) / (this->curtempo * div / 4);
	int cbeat = this->curpos / splperbeat;
	int now = this->curpos % splperbeat;

	if (cbeat == this->current->GetTotalBeats()) {
		this->curpos -= splperbeat * (cbeat);
		cbeat = 0;
		this->Proceed();

		if (!this->drumming) return; 
	}

//	log("div: %d, splpb: %d, cbeat: %d, now: %d", div, splperbeat, cbeat, now);

	if (now < this->buffersize) {
		Beat * b = this->current->GetBeat(cbeat);
		for (int i = 0; i < b->notecount; i++) {
			const CfgSendEvent c = (*this->notes)[b->notes[i]];
			jack->SendMessage(MIDI::Message(c.type, this->curchannel, c.notecc, 127), now);
		}
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
