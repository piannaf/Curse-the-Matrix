/************************************************/
/* File: heartbeat.c                            */
/* Curse The Matrix                             */
/* Copyright (c) Justin Mancinelli              */
/*                                              */
/* Functions to keep track of timing.           */
/*                                              */
/* This is my first foray into C.               */
/* Feel free to copy or modify this to your     */
/* heart's content.                             */
/************************************************/

#include "heartbeat.h"

double ftime(void)
{
    return (double)clock()/(double)CLOCKS_PER_SEC;
}

void heartbeat(void)
{
    static float framestart, frameend;
    
    /*make sure timer increases by at least one unit*/
    do
    {
        frameend = ftime();
    }while(frameend == framestart);

    frametime = frameend - framestart; /*get time between frames*/
    framestart = ftime(); /*start a new frame*/
}
