CXX = g++
CXXFLAGS = -std=c++11 -Wall -Werror -pedantic-errors -DNDEBUG 
CCLINK = $(CXX)
OBJS = ttftps.o
RM = rm -f


ttftps: $(OBJS)
	$(CCLINK) $(CXXFLAGS) -o ttftps $(OBJS)

main.o: ttftps.cpp ttftps.h 
	$(CXX) $(CXXFLAGS) -c main.cpp

clean:
	$(RM) ttftps *.o *~ "#"* core.*

