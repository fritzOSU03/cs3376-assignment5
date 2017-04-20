# File: Makefile for Assignment 5 C++ Programs
# Author: James Fritz
# Date: 04/05/2017
# Purpose: CS3376
# Description: This program compiles and links all of the Assignment 5
#	programs using two-step building process.

PROG = TwoPipesTwoChildren TwoPipesThreeChildren dynpipe
CC = g++
CPPFLAGS = -g -Wall
LDFLAGS = 
OBJS = TwoPipesTwoChildren.o TwoPipesThreeChildren.o DynPipe.o

all : $(PROG)

TwoPipesTwoChildren : TwoPipesTwoChildren.o
	$(CC) $(LDFLAGS) -o TwoPipesTwoChildren TwoPipesTwoChildren.o
TwoPipesThreeChildren : TwoPipesThreeChildren.o
	$(CC) $(CPPFLAGS) -o TwoPipesThreeChildren TwoPipesThreeChildren.o
dynpipe : DynPipe.o
	$(CC) $(CPPFLAGS) -o dynpipe DynPipe.o
TwoPipesTwoChildren.o :
	$(CC) $(CPPFLAGS) -c TwoPipesTwoChildren.cpp
TwoPipesThreeChildren.o :
	$(CC) $(CPPFLAGS) -c TwoPipesThreeChildren.cpp
DynPipe.o :
	$(CC) $(CPPFLAGS) -c DynPipe.cpp
clean :
	rm -f core $(PROG) $(OBJS)
