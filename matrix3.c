/************************************************/
/* File: matrix3.c                              */
/* Curse The Matrix                             */
/* Copyright (c) Justin Mancinelli              */
/*                                              */
/* Uses ncurses to print flows of random        */
/* characters to a random initial column and    */
/* animates the flows as if they were falling.  */
/* There is a simple menu found by pressing 'm' */
/* and a primitive FPS counter.                 */
/*                                              */
/* This is my first foray into C.               */
/* Feel free to copy or modify this to your     */
/* heart's content.                             */
/************************************************/

#include "globals.h"
#include "heartbeat.h"
#include "flows.h"
#include "init_curses.h"
#include "menu.h"

int main(void)
{ 
    float fps; /* frames per second */
    int choice;

    srand(time(NULL));

    init_curses(0);

    total_flows = DEFAULT_NUM_FLOWS;
    init_flows();

    do /*start game loop*/
    {
        if(getch() == 'm')
            choice = getchoice(); /* render menu and get choice from user */

        /*get the clock going*/
        heartbeat();
        
        render_flows();

        /*show fps*/
        fps = 1.0/frametime;
        attron(COLOR_PAIR(1));
        mvprintw(0, 0, "Fps = %f", fps);
        attroff(COLOR_PAIR(1));

        /*update the screen*/
        wnoutrefresh(stdscr);
        doupdate();
        
       /* refresh(); */

    } while(choice != 'Q');


    init_curses(1);

    exit(EXIT_SUCCESS);
}
