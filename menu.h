/************************************************/
/* File: menu.h                                 */
/* Curse The Matrix                             */
/* Copyright (c) Justin Mancinelli              */
/*                                              */
/* Functions to handle the main menu            */
/*                                              */
/* This is my first foray into C.               */
/* Feel free to copy or modify this to your     */
/* heart's content.                             */
/************************************************/

#include <curses.h>
#include "globals.h"
#include "flows.h"

#ifndef MENU_WINDOW
#define MENU_WINDOW
WINDOW *menu_window_ptr;
#endif

extern WINDOW *menu_window_ptr;
/* 
 * defined: flows.h
 * used: execute_choice()
 */
extern int total_flows;

void render_menu(char *options[], int current_highlight);
int getchoice(void);
void execute_choice(int selected, int selected_row);
void get_new_value(int current_highlight); 
void kill_menu(void);
