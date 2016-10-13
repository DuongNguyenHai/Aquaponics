
CC = gcc
CXX = g++

CFLAGS = -Wall -g -fmem-report
# -ftime-report -fmem-report

JSON_C_DIR = ../lib-others/lib-json
JSON_JSMN_DIR = ../lib-others/lib-json-jsmn
LIBRARY_DIR = ../lib

LDFLAGS = -L$(JSON_C_DIR)/lib -L$(JSON_JSMN_DIR)/lib
INCLUDES = -I$(JSON_C_DIR)/include/json-c -I$(JSON_JSMN_DIR)/include/jsmn

LIBS = -ljson-c -ljsmn

# source file
SOURCES = $(LIBRARY_DIR)/seed-json.c $(LIBRARY_DIR)/seed-log.cc seed.cc 

# BUILD = simple.c

# convert name.c to name.o and name.cpp to name.o
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)) )

# LD_LIBRARY_PATH = $(JSON_C_DIR)

# alternative way, u can write directly name of object
# OBJECTS = seed.o ../lib/seed-log.o ../lib/seed-database.o

EXECUTABLE = seed

.PHONY: depend clean

all: $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	@echo $(JSON_C_DIR)
	$(CXX) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(INCLUDES) $(LDFLAGS) $(LIBS)

.cc.o:
	$(CXX) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LDFLAGS) $(LIBS) 

.c.o:
	$(CXX) $(CFLAGS) -c $< -o $@ $(INCLUDES) $(LDFLAGS) $(LIBS)

clean:
	$(RM) *.o $(EXECUTABLE) core

depend: $(SOURCES)
	makedepend $(INCLUDES) $^

#note : viet theo thu tu : *.o $(INCLUDES) $(LDFLAGS) $(LIBS) -o $(EXECUTABLE)