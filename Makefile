CC = g++
CFLAGS = -std=c++11 -pthread
LDFLAGS = 

SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

EXECUTABLE = pomo

CFLAGS += `pkg-config --cflags --libs libnotify`

all: $(SOURCES) $(OBJECTS)
	 
.cpp.o:
	$(CC) $< -o $(EXECUTABLE) $(CFLAGS)
	 
clean:
	rm $(EXECUTABLE)

