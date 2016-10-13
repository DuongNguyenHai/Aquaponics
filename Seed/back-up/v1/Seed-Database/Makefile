
CC = gcc
CXX = g++

CFLAGS = -Wall -g

LFLAGS = -L/usr/local/lib

LIBS = -lmongoc-1.0 -lbson-1.0

INCLUDES = -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/libbson-1.0

# source file
SOURCES = seed.cpp ../lib/seed-log.cpp ../lib/seed-database.c

# convert name.c to name.o and name.cpp to name.o
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)) )

# alternative way, u can write directly name of object
# OBJECTS = seed.o ../lib/seed-log.o ../lib/seed-database.o
.PHONY: depend clean

EXECUTABLE = seed

all: $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(INCLUDES) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CXX) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LFLAGS) $(LIBS) 

.c.o:
	$(CXX) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LFLAGS) $(LIBS)

clean:
	$(RM) *.o $(EXECUTABLE)

depend: $(SOURCES)
	makedepend $(INCLUDES) $(LFLAGS) $(LIBS) $^


#note : viet theo thu tu : *.o $(INCLUDES) $(LFLAGS) $(LIBS) -o $(EXECUTABLE)