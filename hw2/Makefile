# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall -std=c++11
CCLINK = $(CC) -pthread
OBJS = main.o account.o atm.o bank.o log.o
RM = rm -f
# Creating the  executable
Bank: $(OBJS)
	$(CCLINK) -o Bank $(OBJS)
# Creating the object files
Account.o: Account.cpp Account.h
main.o: main.cpp Account.h
Atm.o: Atm.cpp Atm.h
Bank.o: Bank.cpp Bank.h
Log.o: Log.cpp Log.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ Bank "#"* core.*

