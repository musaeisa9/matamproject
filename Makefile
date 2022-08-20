CC=/usr/local/bin/gcc
CXX=/usr/local/bin/g++
OBJS=gcalc.o graph.o main.o
EXEC=gcalc
DEBUG_FLAG=-g
COMP_FLAG=-std=c++11 -fPIC -Wall -Werror -pedantic-errors -DNDEBUG 

$(EXEC) : $(OBJS)
	$(CXX) $(DEBUG_FLAG) $(OBJS) -o $@ 
	
gcalc.o :gcalc.cpp gcalc.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp
graph.o : graph.cpp graph.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp
main.o : gcalc.h
	$(CXX) -c $(DEBUG_FLAG) $(COMP_FLAG) $*.cpp

clean:
	rm -f $(OBJS) 

tar:
	zip gcalc.zip gcalc.cpp gcalc.h graph.h graph.cpp graph.i design.pdf main.cpp test_in.txt test_out.txt gg.gc Makefile

libgraph.a: $(OBJS)
	ar -rs $@ $^