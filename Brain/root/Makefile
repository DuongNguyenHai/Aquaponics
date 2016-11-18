CC = gcc
CXX = g++


DIRLIB = ../lib
# json-c dynamic library directory
JSON_DIR = ../lib/base/lib-json/cJSON

CFLAGS += -std=c++11 -Wall -g `pkg-config --cflags --libs libmongoc-1.0`
LDFLAGS += `pkg-config --cflags --libs libmongoc-1.0` -L$(JSON_DIR)/lib
INCLUDES += -I../lib -I$(JSON_DIR)/include

# link dynamic library
LIBS = -Wl,-rpath='$(JSON_DIR)/lib' -lcJSON
# link static lib
# LIBS = $(JSON_DIR)/lib/libcJSON.a
# source file
SOURCES = $(DIRLIB)/seed-json.cc $(DIRLIB)/seed-log.cc $(DIRLIB)/seed-parse-config.cc $(DIRLIB)/seed-config.cc $(DIRLIB)/seed-database.cc $(DIRLIB)/seed-branch.cc $(DIRLIB)/seed-workspace.cc $(DIRLIB)/TCP-IP.cc main.cc

# convert name.c to name.o and name.cpp to name.o
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)) )
# alternative way, u can write directly name of object
# OBJECTS = seed.o seed-log.o seed-database.o

EXECUTABLE = main

all: $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LDFLAGS) $(INCLUDES) $(LIBS)

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

%.o: %.cc
	$(CXX) -c $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(LIBS) $*.cc -o $*.o

	$(CXX) -MM $(CFLAGS) $(LDFLAGS) $(INCLUDES) $(LIBS) $*.cc > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	$(RM) *.o $(EXECUTABLE)


#note : viet theo thu tu : *.o $(INCLUDES) $(LDFLAGS) $(LIBS) -o $(EXECUTABLE)