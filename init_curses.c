/************************************************/
/* File: init_curses.c                          */
/* Curse The Matrix                             */
/* Copyright (c) Justin Mancinelli              */
/*                                              */
/* Functions to initialize and end the ncurses  */
/* session.                                     */
/*                                              */
/* This is my first foray into C.               */
/* Feel free to copy or modify this to your     */
/* heart's content.                             */
/************************************************/

#include "init_curses.h"

void init_curses(bool kill)
{
    if(!kill)
    {
        initscr(); /*initialize curses mode*/

        if(has_colors() == TRUE)
            start_color();

        init_pair(1, COLOR_GREEN, COLOR_BLACK); /*setup classic green on black*/

        curs_set(0); /*we shouldn't see the cursor */
        noecho(); /*we shouldn't see what the user presses*/
        cbreak(); /*disable line buffering*/
        nodelay(stdscr, 1); /*getch is now non-bloking, returning ERR if no input*/
        keypad(stdscr, TRUE);
    }
    else
    {
        keypad(stdscr, FALSE);
        curs_set(1); /*we should see the cursor*/
        echo(); /*we should see what the user presses*/
        nocbreak(); /*enable line buffering*/
        endwin(); /*exit curses mode*/
    }
}
