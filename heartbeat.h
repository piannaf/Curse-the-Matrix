/************************************************/
/* File: heartbeat.h                            */
/* Curse The Matrix                             */
/* Copyright (c) Justin Mancinelli              */
/*                                              */
/* Functions to keep track of timing.           */
/*                                              */
/* This is my first foray into C.               */
/* Feel free to copy or modify this to your     */
/* heart's content.                             */
/************************************************/

#include <time.h> 

#ifndef FRAMETIME
#define FRAMETIME
float frametime;
#endif

extern float frametime; 

double ftime(void);

void heartbeat(void);
