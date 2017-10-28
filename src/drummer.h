#pragma once

#include "log.h"
#include "types.h"
#include "jacker.h"
#include "loopmgr.h"
#include "notemap.h"

class Drummer { 
	public:
		Drummer();
		~Drummer();
		void Initialize(LoopManager & loops, NoteMap & notes);
		void ChangeLoop(loopref_t loop);
		void SetEmitChannel(int channel);
		void SetTempo(int tempo);
		void StopDrumming();
		void StartDrumming();
		void UpdateParams(int splrate, int buffersize);
		void TapTempo(jack_nframes_t time);

		void Drum(Jacker * jack);
	private:
		void Proceed();

		LoopManager * loops;
		NoteMap * notes;

		DrumLoop * current;
		DrumLoop * next;

		int curtempo;
		int curchannel;

		int nexttempo;
		int nextchannel;

		int splrate;
		int buffersize;
		bool drumming;

		int curpos;

		// tap tempo vars:
		bool counttempo;
		int ttsamples;
		int ttlastclick;
		int ttfirstclick;
		int ttclicks;
};
