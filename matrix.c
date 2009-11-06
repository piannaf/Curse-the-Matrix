/************************************************/
/* File: matrix.c                               */
/* Curse The Matrix                             */
/* Copyright (c) Justin Mancinelli              */
/*                                              */
/* Simply uses ncurses to print a random        */
/* character to the next column of the terminal */
/* then loops back around when it reaches the   */
/* end.                                         */
/*                                              */
/* This is my first foray into C.               */
/* Feel free to copy or modify this to your     */
/* heart's content.                             */
/************************************************/

#include <curses.h>
#include <stdlib.h>
#include <time.h>

#define ASCII_START 40
#define ASCII_END 50

void init_curses(bool exit)
{
    if(!exit)
    {
        initscr(); //initialize curses mode
        curs_set(0); //we shouldn't see the cursor
        noecho(); //we shouldn't see what the user presses
        cbreak(); //disable line buffering
    }
    else
    {
        curs_set(1); //we should see the cursor
        echo(); //we should see what the user presses
        nocbreak(); //enable line buffering
        endwin(); //exit curses mode
    }
}

int Random(int min, int max)
{
    return (rand()%max)+min;
}

//void heartbeat(void)
//{
//    tick = time


int main()
{
    init_curses(0);

    nodelay(stdscr, 1); //getch is now non-bloking, returning ERR if no input
    init_pair(1, COLOR_GREEN, COLOR_BLACK); //setup classic green on black

    int max_x, max_y; //for storing screen dimensions
    int row = 0, col = 0; //initialze x and y position variables;
    char symbol; //initialize starting character
    char ch;
    getmaxyx(stdscr, max_y, max_x); //get screen dimensions

    while(ch = getch() ) //start game loop
    {
        if(ch != ERR)
            break;

        symbol = (char)Random(ASCII_START, ASCII_END);
        row = max_y / 2; //keep y-pos at center of screen for now
        mvprintw( row, col, "%c", symbol); //print the current symbol
        refresh(); //update the screen
        
        //setup for next loop
        usleep(150000); //wait before erase 
        mvprintw( row, col, " "); //erase previous place
        
/* depricated, use random symbol in range instead
        symbol++; //go to next ascii character
        if(symbol == ASCII_END) symbol = ASCII_START; //if we're at the end, go back to the beginning
*/
        col++; //go to the next x-position
        if(col == max_x) col = 0; //if we're at the edge of the screen, go back to the other edge
    }

    init_curses(1);

    exit(EXIT_SUCCESS);
}
