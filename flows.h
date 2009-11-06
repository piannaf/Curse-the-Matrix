/************************************************/
/* File: flows.h                                */
/* Curse The Matrix                             */
/* Copyright (c) Justin Mancinelli              */
/*                                              */
/* Datastructure for a character flow.          */
/* Functions to handle the character flows.     */
/*                                              */
/* This is my first foray into C.               */
/* Feel free to copy or modify this to your     */
/* heart's content.                             */
/************************************************/

#include "init_curses.h"
#include "globals.h"
#include "heartbeat.h"

#ifndef ASCII_START
#define ASCII_START 40
#endif

#ifndef ASCII_END
#define ASCII_END 50
#endif

#ifndef MAX_SPEED
#define MAX_SPEED 16
#endif

#ifndef DEFAULT_NUM_FLOWS
#define DEFAULT_NUM_FLOWS 25
#endif

#ifndef TOTAL_NUM_FLOWS
#define TOTAL_NUM_FLOWS
int total_flows;
#endif

extern int total_flows;

#ifndef TEXT_FLOW_STRUCT
#define TEXT_FLOW_STRUCT
typedef struct text_flow
{
    int x_pos;
    float y_pos;
    float y_pos_old;
    int curr_row;
    int prev_row;
    int len;
    int color;
    int velocity;
    char *text;
}flow;
#endif

#ifndef FLOW_PTR
#define FLOW_PTR
flow **mstr_ptr;
#endif

int Random(int min, int max);
void init_flows(void);
void generate_mstr(flow *mstr, int max_y, int max_x );
void print_mstr(flow *mstr, int max_y);
void render_flows(void);
void change_num_flows(int new_num_flows);
