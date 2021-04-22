CC = g++
CFLAGS = -O3
DEBUG_CFLAGS = -g -Wall -pedantic -Wextra -ggdb
LIBS = -lglut -lm -lGLU -lGL
EXECUTABLE = rick
SOURCES = $(wildcard *.cpp)        
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

all: rick

%.o : %.cpp %.h
	$(CC) -c $(CFLAGS) $(INCDIRS) $(INCDIRGL) $*.cpp

rick: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LIBDIRS) $(LIBS)
	strip --strip-all $(EXECUTABLE)

clean:
	rm -f *.o $(EXECUTABLE) $(DEBUG_EXECUTABLE)
