#pragma once

#define DEBUG

#ifndef LOG_H
#define LOG_H

#include <stdio.h>


#ifdef DEBUG
#define STRINGIFY2(x) #x
#define STRINGIFY(x) STRINGIFY2(x)

#define log(msg,...) printf("\033[1;30m" __FILE__ ":" STRINGIFY(__LINE__) ":\t" msg "\033[0m\n",  ## __VA_ARGS__)
#else
#define log(msg,...) 
#endif



#endif 
