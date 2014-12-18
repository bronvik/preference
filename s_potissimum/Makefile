CC=g++
CFLAGS=-c -Wall -std=c++11 
LDFLAGS=-lboost_system -lboost_thread -lpthread -std=c++11
SOURCES=player.cpp main.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=server

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
