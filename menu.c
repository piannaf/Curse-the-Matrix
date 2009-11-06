/************************************************/
/* File: menu.c                                 */
/* Curse The Matrix                             */
/* Copyright (c) Justin Mancinelli              */
/*                                              */
/* Functions to handle the main menu            */
/*                                              */
/* This is my first foray into C.               */
/* Feel free to copy or modify this to your     */
/* heart's content.                             */
/************************************************/

#include "menu.h"

void render_menu(char *options[], int current_highlight)
{
    int current_row = 0;
    char **option_ptr;
    char *txt_ptr;

    /* Options */
    option_ptr = options;
    while(*option_ptr)
    {
        if(current_row == current_highlight) wattron(menu_window_ptr, A_STANDOUT);
        txt_ptr = options[current_row];

        if(txt_ptr[0] == 'N')
            mvwprintw(menu_window_ptr, 4 + current_row, 2, "%26d", total_flows);
        else if(txt_ptr[0] == 'M')
            mvwprintw(menu_window_ptr, 4 + current_row, 2, "%26d", 0);

        mvwprintw(menu_window_ptr, 4 + current_row, 2, "%s", txt_ptr);

        txt_ptr++;
        if(current_row == current_highlight)
            wattroff(menu_window_ptr, A_STANDOUT);
        current_row++;
        option_ptr++;
    }

    wrefresh(menu_window_ptr);
}

int getchoice(void)
{
    char *choices[] = 
    {
        "Take the red pill",
        "Number of flows",
        "Max framerate",
        "Quit",
        0,
    };
    static int selected_row = 0;
    int max_row = 0;
    int max_y, max_x;
    char **option;
    int selected;
    int key;

    option = choices;
    while(*option)
    {
        max_row++;
        option++;
    }

    /* display menu window */
    getmaxyx(stdscr, max_y, max_x);
    menu_window_ptr = newwin(10,30, (max_y/2) - 5 ,(max_x/2) - 10);
    box(menu_window_ptr, '|', '-');

    /* Title */
    mvwprintw(menu_window_ptr, 2, (30/2) - 8, "%s", "Enter the Matrix");

    keypad(menu_window_ptr, TRUE);
    nodelay(menu_window_ptr, FALSE);
    
    while(key != 'q' && selected != 'Q' && selected != 'T')
    {
        if(key == KEY_UP)
        {
            /* wrap top to bottom */
            if(selected_row == 0)
                selected_row = max_row - 1;
            else
                selected_row--;
        }
        if(key == KEY_DOWN)
        {
            /* wrap bottom to top */
            if(selected_row == (max_row - 1))
                selected_row = 0;
            else
                selected_row++;
        }
        
        render_menu(choices, selected_row);
        key = wgetch(menu_window_ptr);
        
        if(key == KEY_ENTER || key == '\n')
        {
            selected = *choices[selected_row];
            execute_choice(selected, selected_row);
        }
    }

    nodelay(menu_window_ptr, TRUE);

    if(key == 'q')
        selected = 'Q';

    /* choice has been made, get rid of the menu */
    kill_menu();

    return selected;
}

void execute_choice( int choice, int selected_row)
{
    int key = 0;
    int new_num_flows;

    switch(choice)
    {
        case 'T': /* Take the Red Pill */
            break;
        case 'N': /* Number of Flows */
            new_num_flows = total_flows;
            wattron(menu_window_ptr, A_BOLD);

            while(key != KEY_ENTER && key != '\n')
            {
                if(key == KEY_UP && new_num_flows < 100)
                    new_num_flows++;
                if(key == KEY_DOWN && new_num_flows > 0)
                    new_num_flows--;

                mvwprintw(menu_window_ptr, 4 + selected_row, 2 + 15, "%11d", new_num_flows);
                wrefresh(menu_window_ptr);

                key = wgetch(menu_window_ptr);
            }

            if(new_num_flows != total_flows)
            {
                change_num_flows(new_num_flows);
            }

            wattroff(menu_window_ptr, A_BOLD);
            break;
        case 'M': /* Max Framerate */
            break;
        case 'Q': /* Quit */
            break;
    }
}

void kill_menu(void)
{
    delwin(menu_window_ptr);
    touchwin(stdscr);
    refresh();
}
