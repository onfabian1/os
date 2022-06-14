# Makefile for the tftp program
CC = g++
CFLAGS = -g -Wall -std=c++11
CCLINK = $(CC) 
OBJS = main.o 
RM = rm -f
# Creating the executable
ttftps: $(OBJS)
	$(CCLINK) -o ttftps $(OBJS)
# Creating the object files
main.o: main.cpp
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ ttftps "#"* core.*

