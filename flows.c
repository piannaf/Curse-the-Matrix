/************************************************/
/* File: flows.c                                */
/* Curse The Matrix                             */
/* Copyright (c) Justin Mancinelli              */
/*                                              */
/* Functions to handle the character flows.     */
/*                                              */
/* This is my first foray into C.               */
/* Feel free to copy or modify this to your     */
/* heart's content.                             */
/************************************************/

#include "flows.h"

int Random(int min, int max)
{
    return (rand()%max)+min;
}

void init_flows()
{
    int num_flows; /*count created flows*/
    int max_y, max_x; /* screen dimensions */

    /* check size of screen */
    getmaxyx(stdscr, max_y, max_x);

    /* create array of flow pointers */
    mstr_ptr = (flow **)malloc(sizeof(flow *)*total_flows);
    if(mstr_ptr == NULL)
    {
        init_curses(1);
        exit(EXIT_FAILURE);
    }

    /* initialize array of flow pointers with random flows */
    for(num_flows = 0; num_flows < total_flows; num_flows++)
    {
        mstr_ptr[num_flows] = (flow *)malloc(sizeof(flow));
        if(mstr_ptr[num_flows] == NULL)
        {
            init_curses(1);
            exit(EXIT_FAILURE);
        }

        generate_mstr(mstr_ptr[num_flows], max_y, max_x);
    }
}

void generate_mstr( flow *mstr, int max_y, int max_x )
{
    int flow_offset;
    char *text_ptr;

    mstr->x_pos = Random(0, max_x); /*place in random column*/
    mstr->y_pos = 0; /*always start at top of screen*/
    mstr->y_pos_old = 0;
    mstr->curr_row = 0;
    mstr->prev_row = 0;
    mstr->len = Random(4, max_y); /*keep length between height of screen and 4 characters*/
    mstr->color = Random(1, 2) - 1; /*light, dark*/
    mstr->velocity = Random(4, MAX_SPEED); /*set velocity in range*/
    
    /*allocate space for string of symbols*/
    mstr->text = (char *)malloc(sizeof(char) * (mstr->len + 1));
    if(mstr->text == NULL) exit(EXIT_FAILURE);

    /* generate string of symbols */
    text_ptr = mstr->text;
    for(flow_offset = 0; flow_offset < mstr->len; flow_offset++) 
    {
        text_ptr[flow_offset] = Random(ASCII_START, ASCII_END); 
    }
}

void render_flows()
{
    int num_flows; /* count flows */
    int max_y, max_x; /* screen dimensions */

    /* check size of screen */
    getmaxyx(stdscr, max_y, max_x);

    /* print flows */
    for(num_flows = 0; num_flows < total_flows; num_flows++)
    {
        /*if current string is fully below the window then free the string and generate new values*/
        if(mstr_ptr[num_flows]->curr_row - mstr_ptr[num_flows]->len > max_y + 1) 
        {
            free(mstr_ptr[num_flows]->text);
            generate_mstr(mstr_ptr[num_flows], max_y, max_x);
        }
        else /* print with current values */
            print_mstr(mstr_ptr[num_flows], max_y);
    }
}

void print_mstr( flow *mstr, int max_y)
{
    int flow_offset;
    char *text_ptr;

    /*update output if something moved*/
    if(mstr->curr_row != mstr->prev_row)
    { 
        /* erase last character of the old flow if showing*/
        if(mstr->prev_row >= mstr->len) 
            mvaddch(mstr->curr_row - mstr->len, mstr->x_pos, ' ');

        /*set light or dark color*/
        if(mstr->color==1)
            attron(COLOR_PAIR(1) | A_BOLD);
        else
            attron(COLOR_PAIR(1) | A_NORMAL);
        
        /*get screen ready*/
        /* partially above window */
        if(mstr->curr_row < mstr->len)
        {
            /* point to first character */
            text_ptr = mstr->text; 
            for(flow_offset = 0; flow_offset < mstr->curr_row; flow_offset++) 
                mvaddch(mstr->curr_row - flow_offset, mstr->x_pos, text_ptr[flow_offset]); 
        }
        /* partially below window */
        else if(mstr->curr_row >= max_y )
        {
            /*point to the lowest visible character*/
            text_ptr = mstr->text + (mstr->curr_row - max_y + 1 ); 
            for(flow_offset = 0; flow_offset < ((mstr->len - 1) - (mstr->curr_row - max_y - 1) - 1); flow_offset++)
                mvaddch((max_y - 1) - flow_offset, mstr->x_pos, text_ptr[flow_offset]); 
        }
        /*all characters are within the window*/
        else
        {
            /*point to first character*/
            text_ptr = mstr->text; 
            for(flow_offset = 0; flow_offset < mstr->len; flow_offset++)
                mvaddch(mstr->curr_row - flow_offset, mstr->x_pos, text_ptr[flow_offset]); 

        }
        attroff(COLOR_PAIR(1) | A_BOLD);
    }

    /* setup positions */
    
    mstr->y_pos_old = mstr->y_pos;
    mstr->y_pos = mstr->y_pos_old + mstr->velocity*frametime;
    
    mstr->prev_row = mstr->curr_row; 
    mstr->curr_row = (int)mstr->y_pos;
}

void change_num_flows(int new_num_flows)
{
    int num_flows;
    int max_y, max_x; /* screen dimensions */

    /* check size of screen */
    getmaxyx(stdscr, max_y, max_x);

    if(new_num_flows != total_flows)
    {
        mstr_ptr = (flow **)realloc(mstr_ptr, sizeof(flow *)*new_num_flows);
        if(mstr_ptr == NULL)
        {
            init_curses(1);
            exit(EXIT_FAILURE);
        }

        /* initialize new flow pointers with random flows*/
        if(new_num_flows > total_flows)
        {
            for(num_flows = total_flows; num_flows < new_num_flows; num_flows++) /* initialize extra spaces */
            {
                mstr_ptr[num_flows] = (flow *)malloc(sizeof(flow));
                if(mstr_ptr[num_flows] == NULL)
                {
                    init_curses(1);
                    exit(EXIT_FAILURE);
                }

                generate_mstr(mstr_ptr[num_flows], max_y, max_x);
            }
        }
        total_flows = new_num_flows;
    }
}
