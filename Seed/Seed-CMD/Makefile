CC = gcc
CXX = g++

CFLAGS = -Wall -g

LDFLAGS =

LIBS =

INCLUDES =

# source file
SOURCES = seed.cc ../lib/seed-log.cc ../lib/TCP-IP.cc ../lib/cmd-config.cc

# convert name.c to name.o and name.cpp to name.o
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)) )

# alternative way, u can write directly name of object
# OBJECTS = seed.o ../lib/seed-log.o ../lib/seed-database.o

EXECUTABLE = seed

all: $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(INCLUDES) $(LDFLAGS) $(LIBS)

.cc.o:
	$(CXX) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LDFLAGS) $(LIBS) 

.c.o:
	$(CXX) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LDFLAGS) $(LIBS)

clean:
	$(RM) *.o $(EXECUTABLE)


#note : viet theo thu tu : *.o $(INCLUDES) $(LDFLAGS) $(LIBS) -o $(EXECUTABLE)