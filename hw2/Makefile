# Makefile for the smash program
CC = g++
CFLAGS = -g -Wall -std=c++11
CCLINK = $(CC) -pthread
OBJS = main.o account.o atm.o bank.o log.o
RM = rm -f
# Creating the executable
Bank: $(OBJS)
	$(CCLINK) -o Bank $(OBJS)
# Creating the object files
main.o: main.cpp account.o atm.o bank.o log.o
account.o: account.cpp account.h
atm.o: atm.cpp atm.h
bank.o: bank.cpp bank.h
log.o: log.cpp log.h
# Cleaning old files before new make
clean:
	$(RM) $(TARGET) *.o *~ Bank "#"* core.*

