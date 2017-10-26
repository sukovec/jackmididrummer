Jack Midi Drummer
===
Now called "AutoMidiDrummer", but it will be renamed

The goal
===
The goal of this small piece of software is to make simple textually configurable drum machine where individual drum loops can be switched using midi. 

Current status
===
It is pretty basic piece of software... but it works. However, stdout is full of debug output, some features are not implemented yet

Prerequisities
===
* Jack
* Cmake (build time)
* C++11 compatible compiler

How to build
===
It's basically just repository checkout and cmake call
```bash
$ git clone git@github.com:sukovec/jackmididrummer.git
$ cd jackmididrummer
$ mkdir build ; cd build
$ cmake ..
$ mv config.example config
$ ./automididrummer # yes, it's called like this for now :)
```

Configuration
===
Configuration is pretty straightforward, look into config.example
