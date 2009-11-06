/************************************************/
/* File: matrix.c                               */
/* Curse The Matrix                             */
/* Copyright (c) Justin Mancinelli              */
/*                                              */
/* Uses ncurses to print flows of random        */
/* characters to a random initial column and    */
/* animates the flows as if they were falling.  */
/* There is also a primitive FPS counter.       */
/*                                              */
/* This is my first foray into C.               */
/* Feel free to copy or modify this to your     */
/* heart's content.                             */
/************************************************/

#include <curses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define ASCII_START 40
#define ASCII_END 50
#define MAX_SPEED 16
#define DEFAULT_NUM_FLOWS 10

void init_curses(bool exit)
{
    if(!exit)
    {
        initscr(); //initialize curses mode

        if(has_colors() == TRUE)
            start_color();

        curs_set(0); //we shouldn't see the cursor
        noecho(); //we shouldn't see what the user presses
        cbreak(); //disable line buffering
        nodelay(stdscr, 1); //getch is now non-bloking, returning ERR if no input
    }
    else
    {
        curs_set(1); //we should see the cursor
        echo(); //we should see what the user presses
        nocbreak(); //enable line buffering
        endwin(); //exit curses mode
    }
}

double ftime(void)
{
    return (double)clock()/(double)CLOCKS_PER_SEC;
}

static float frametime; //used in heartbeat and to calculate FPS

void heartbeat(void)
{
    static float framestart, frameend;
    
    //make sure timer increases by at least one unit
    do
    {
        frameend = ftime();
    }while(frameend == framestart);

    frametime = frameend - framestart; //get time between frames
    framestart = ftime(); //start a new frame
}

int Random(int min, int max)
{
    return (rand()%max)+min;
}

typedef struct text_flow 
{
    int x_pos; //which column the flow is in
    float y_pos; //which row the flow is at (first symbol)
    float y_pos_old;
    int len; //the length of the flow
    int color; //color of the flow
    int velocity; //multiply by interframe time to get new position
    char *text;
}flow;

void generate_mstr( flow *mstr, int max_y, int max_x )
{
    mstr->x_pos = Random(0, max_x); //place in random column
    mstr->y_pos = 0; //always start at top of screen
    mstr->y_pos_old = 0;
    mstr->len = Random(4, max_y); //keep length between height of screen and 4 characters
    mstr->color = Random(1, 2) - 1; //light, dark
    mstr->velocity = Random(4, MAX_SPEED); //set velocity in range
    
    //generate the string of symbols
    mstr->text = (char *)malloc(sizeof(char) * (mstr->len)); //allocate length of the string bytes
    if(mstr->text == NULL) exit(EXIT_FAILURE);

    int i;
    char *text_ptr;
    text_ptr = mstr->text; //don't mess with the original pointer
    for(i = 0; i <= mstr->len; i++)
    {
        *text_ptr++ = Random(ASCII_START, ASCII_END); //insert new symbol then increment pointer
    }
}

void print_mstr( flow *mstr, int max_y, int max_x)
{
    int flow_offset;
    char *text_ptr;

    //update output if something moved
    if((int)mstr->y_pos_old != (int)mstr->y_pos)
    { 
        //is any old output lingering?
        if(mstr->y_pos_old >= mstr->len) //is the last character of the old flow showing?
            mvaddch((int)mstr->y_pos_old - mstr->len, mstr->x_pos, ' '); //then erase it


        /* * * * * * * * * *
         *  New flow logic *
         * * * * * * * * * */

        //set light or dark color
        if(mstr->color==1)
            attron(COLOR_PAIR(1) | A_BOLD);
        else
            attron(COLOR_PAIR(1) | A_NORMAL);
        
        //get screen ready
        if(mstr->y_pos > max_y ) //are some characters below the window?
            text_ptr = 1 + mstr->text + ((int)mstr->y_pos - max_y) ; //then point to the lowest visible character
        else
            text_ptr = mstr->text; //point to the first character

        // flow_offset: keep track of the current position inside the string
        // flow_offset <= (int)mstr->y_pos: true when writing within the visible window
        // flow_offset <= mstr->len: true when pointing within the text string
        for(flow_offset = 0; flow_offset <= mstr->y_pos && flow_offset <= mstr->len; flow_offset++)
            mvaddch((int)mstr->y_pos - flow_offset, mstr->x_pos, *text_ptr++);//place current character at current position 

        attroff(COLOR_PAIR(1) | A_BOLD);
        refresh(); //update the screen      
    }
    
    mstr->y_pos_old = mstr->y_pos; //reset the old position
    mstr->y_pos = mstr->y_pos_old + mstr->velocity*frametime; //set the next position
    
    if((int)mstr->y_pos - mstr->len > max_y) //is the current string fully below the window?
    {
        free(mstr->text); //free flow's string from memory
        generate_mstr(mstr, max_y, max_x); //generate new flow at current location
    }
}

int main()
{ 
    srand(time(NULL));
    init_curses(0);

    init_pair(1, COLOR_GREEN, COLOR_BLACK); //setup classic green on black

    char ch; //getch()

    //initialize screen dimensions
    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x); 
    
    //frames per second
    float fps;

    //initialize flow
    flow **mstr_ptr = (flow **)malloc(sizeof(flow *));
    if(mstr_ptr == NULL) 
    {
        init_curses(1);
        exit(EXIT_FAILURE);
    }

    int num_flows;
    for(num_flows = 0; num_flows < DEFAULT_NUM_FLOWS; num_flows++)
    {
        mstr_ptr[num_flows] = (flow *)malloc(sizeof(flow)); //make space for flow
        if(mstr_ptr[num_flows] == NULL) 
        {
            init_curses(1);
            exit(EXIT_FAILURE);
        }

        generate_mstr(mstr_ptr[num_flows], max_y, max_x); //generate a flow at this position
    }

    //mstr_ptr -= DEFAULT_NUM_FLOWS; //reset the pointer

    while((ch = getch()) != 'q') //start game loop
    {
        //get the clock going
        heartbeat();
        
        //print the flows
        for(num_flows = 0; num_flows < DEFAULT_NUM_FLOWS; num_flows++)
            print_mstr(mstr_ptr[num_flows], max_y, max_x); //print the flow at this position

        //show fps
        fps = 1.0/frametime;
        attron(COLOR_PAIR(1));
        mvprintw(max_y - 1, 0, "Fps = %f", fps);
        attroff(COLOR_PAIR(1));
    }

    //for(num_flows = 0; num_flows < DEFAULT_NUM_FLOWS; num_flows++)
    //    free( mstr_ptr++ ); //free memory from all generated flows

    init_curses(1);

    exit(EXIT_SUCCESS);
}
