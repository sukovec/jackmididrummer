#include "jacker.h"

int jacker_process (jack_nframes_t nframes, void *arg) {
	return ((Jacker*)arg)->Process(nframes);
}

void jacker_shutdown(void * arg) {
	((Jacker*)arg)->Shutdown();
}

int jacker_bufsize_changed(jack_nframes_t size, void * arg) {
	((Jacker*)arg)->BufferSizeChanged(size);
}

void jacker_error(const char * out) {
	printf("\033[1;31mJack error: %s\033[0m\n", out);
}

Jacker::Jacker() {
	log("Jacker::Jacker()");

	this->jackcl = nullptr;
	this->outbuffer = nullptr;

}

Jacker::~Jacker() {
	log("Jacker::~Jacker()");
	if (this->jackcl != nullptr)
		this->Close();
}

void Jacker::Open(const char * clname, const char ** outputs, int outputcount) {
	log("Jacker::Open()");
	jack_status_t status;
	this->jackcl = jack_client_open(clname, JackNullOption, &status, nullptr);

	if (this->jackcl == nullptr) 
		throw std::invalid_argument("Cannot initialize jack interface");

	log("Jack opened successfully");

	jack_set_process_callback(this->jackcl, jacker_process, this);
	jack_on_shutdown(this->jackcl, jacker_shutdown, this);
	jack_set_error_function(jacker_error);
	jack_set_info_function(jacker_error);

	this->buffersize = jack_get_buffer_size(this->jackcl);
	this->samplerate = jack_get_sample_rate(this->jackcl);

	printf("Jack info: buffersize = %d, samplerate = %d\n", this->buffersize, this->samplerate);

	this->outputcount = outputcount;
	this->CreatePorts(outputs, outputcount);
}

void Jacker::Close() {
	log("Jacker::Close()");

	if (this->jackcl != nullptr)
		jack_client_close(this->jackcl);

	this->jackcl = nullptr;
}

void Jacker::Run() {
	log("Jacker::Run()");
	if (jack_activate(this->jackcl) != 0) {
		throw std::runtime_error("Cannot activate jack client");
	}

	printf("Jack client activated\n");
}

// TODO: maybe optimalize it?
int Jacker::Process(jack_nframes_t nframes) {
	void * in = jack_port_get_buffer (this->input, nframes);
	for (int i = 0; i < this->outputcount; i++) {
		this->outbuffer[i] = jack_port_get_buffer (this->outputs[i], nframes);
		jack_midi_clear_buffer(this->outbuffer[i]);
	}

	jack_nframes_t cnt = jack_midi_get_event_count(in);

	jack_midi_event_t jme;
	for (int i = 0; i < cnt; i++)  {
		jack_midi_event_get(&jme, in, i);

		this->ProcessMessage(jme);
	}


	if (this->generatorcallback.IsSet())
		this->generatorcallback(this);

	return 0;
}

void Jacker::ProcessMessage(jack_midi_event_t & evt) {
	MIDI::Message msg = MIDI::Message::FromBuffer(evt.buffer, evt.size);
	if (msg.IsValid()) { // Can I definitely ignore time member in evt? ... no, I cannot :(
		if (this->msgcallback.IsSet())
			this->msgcallback(msg, evt.time);
	} else {
		log("Received invalid/unknown message");
	}
}

void Jacker::Shutdown() {
	log("Jacker::Shutdown()");
}
int Jacker::BufferSizeChanged(jack_nframes_t size) {
	log("Jacker::BufferSizeChanged(%d)", size);
	
	this->buffersize = size;
}

void Jacker::SetCallback(MessageCallback callback) {
	log("Jacker::SetCallback()");
	this->msgcallback = callback;
}

void Jacker::SetGenerator(GeneratorCallback callback) { 
	log("Jacker::SetGenerator()");
	
	this->generatorcallback = callback;
}

void Jacker::SendMessage(MIDI::Message msg, int time, outputref_t out) {
	int32_t buffer; // 4 bytes should be enough for notes or cc/pc
	int dtsz = msg.Encode(&buffer, sizeof(buffer));
	if (dtsz < 1) {
		log("Jacker::SendMessage: Invalid buffer passed");
		return;
	}

	jack_midi_event_write(this->outbuffer[out], time, (jack_midi_data_t*)&buffer, (size_t)dtsz);
}

int Jacker::GetBufferSize() {
	return this->buffersize;
}

int Jacker::GetSampleRate() {
	return this->samplerate;
}

void Jacker::CreatePorts(const char ** outputs, int outputcount) {
	log("Jacker::CreatePorts()");

	this->input = jack_port_register (this->jackcl, "input", JACK_DEFAULT_MIDI_TYPE , JackPortIsInput, 0);
	if (this->input == nullptr) 
		throw std::runtime_error("Creation of jack input port was unsuccessfull");

	this->outputs = new jack_port_t*[outputcount];
	this->outbuffer = new void*[outputcount];
	for (int i = 0; i < outputcount; i++) {
		printf("Creating output %d: %s\n", i, outputs[i]);
		this->outputs[i] = jack_port_register (this->jackcl, outputs[i], JACK_DEFAULT_MIDI_TYPE , JackPortIsOutput, 0);
		if (this->outputs[i] == nullptr) 
			throw std::runtime_error("Creation of jack output port was unsuccessfull");
	}

	printf("Input port: %s\n", jack_port_name(this->input)); 
	for (int i = 0; i < outputcount; i++) 
		printf("Output port: %s\n", jack_port_name(this->outputs[i]));

}

void Jacker::ConnectPorts(const char * portname, JackPortType type, outputref_t out) {
	printf("Connecting %s ports matching '%s'\n", type == JackPortType::Input ? "input" : "output", portname);
	const char ** ports = jack_get_ports(this->jackcl, portname, "midi", type == JackPortType::Input ? JackPortIsOutput : JackPortIsInput);

	if (ports == nullptr) {
		printf("\tSearching for port '%s' has \033[31mfailed\033[0m\n", portname);
		return;
	}

	int ret;
	if (type == JackPortType::Input) {
		for (int i = 0; ports[i] != nullptr; i++) {
			ret = jack_connect(this->jackcl, ports[i], jack_port_name(this->input));
			printf("\tConnecting input '%s' -> '%s' %s\n", ports[i], jack_port_name(this->input), ret == 0 ? "was successfull" : "\033[31mfailed\033[0m");
		}
	} else {
		for (int i = 0; ports[i] != nullptr; i++) {
			ret = jack_connect(this->jackcl, jack_port_name(this->outputs[out]), ports[i]);
			printf("\tConnecting output '%s' -> '%s' %s\n", jack_port_name(this->outputs[out]), ports[i], ret == 0 ? "was successfull" : "\033[31mfailed\033[0m");
		}
	}

	jack_free(ports);
}
