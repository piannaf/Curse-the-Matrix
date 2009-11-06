##################################################
## File: Makefile                               ##
## Curse The Matrix                             ##
## Copyright (c) Justin Mancinelli              ##
##                                              ##
## This is my first foray into C.               ##
## Feel free to copy or modify this to your     ##
## heart's content.                             ##
##################################################
all: matrix3

# use gcc
CC = gcc

# options for development
CFLAGS = -g -W -Wall -pedantic -ansi -lncurses

# where are the include files
INCLUDE = .

# installation directory
INSTDIR = /usr/local/bin

3matrix3: matrix3.o heartbeat.o flows.o init_curses.o menu.o
	$(CC) -o matrix3 matrix3.o heartbeat.o flows.o init_curses.o menu.o -lncurses

matrix3.o: matrix3.c

heartbeat.o: heartbeat.c heartbeat.h

flows.o: flows.c flows.h heartbeat.h

init_curses.o: init_curses.c init_curses.h

menu.o: menu.c menu.h flows.h 

clean:
	-rm matrix3.o heartbeat.o flows.o init_curses.o menu.o

uninstall:
	-rm 3matrix3

install: 3matrix3
	@if [ -d $(INSTDIR) ]; \
		then \
		cp 3matrix3 $(INSTDIR);\
		chmod a+x $(INSTDIR)/3matrix3;\
		chmod og-w $(INSTDIR)/3matrix3;\
		echo "Installed in $(INSTDIR)";\
	else \
		echo "Sorry, $(INSTDIR) does not exist";\
	fi
