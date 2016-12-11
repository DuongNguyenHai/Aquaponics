CC = gcc
CXX = g++

ROOT_DIR:=$(shell dirname `pwd`)

DIRLIB = $(ROOT_DIR)/lib
# json-c dynamic library directory
JSON_DIR = $(DIRLIB)/base/lib-json/cJSON

CFLAGS += -std=c++11 -Wall -g `pkg-config --cflags --libs libmongoc-1.0`
LDFLAGS += `pkg-config --cflags --libs libmongoc-1.0` -lpthread -L$(JSON_DIR)/lib
INCLUDES += -I$(DIRLIB) -I$(JSON_DIR)/include
# I dont know why that I have to put -lpthread in LDFLAGS but CFLAGS. It -lpthread is in CFLAGS. It will not working
# Oh. The order of command line of compiler is matter. so -lpthread must after $(OBJECT). see my note to learn more
# link dynamic library
LIBS = -Wl,-rpath='$(JSON_DIR)/lib' -lcJSON
# link static lib.. there is some error when include static ....
# LIBS = $(JSON_DIR)/lib/libcJSON.a
# LIBS = -lcJSON
# source file
SOURCES = $(DIRLIB)/seed-json.cc $(DIRLIB)/seed-log.cc $(DIRLIB)/seed-parse-config.cc $(DIRLIB)/seed-config.cc $(DIRLIB)/seed-database.cc $(DIRLIB)/seed-branch.cc $(DIRLIB)/seed-workspace.cc $(DIRLIB)/TCP-UDP.cc main.cc

# convert name.c/name.cc/name.cpp/... to name.o
OBJECTS = $(addsuffix .o, $(basename $(SOURCES)) )
# alternative way, u can write directly name of object
# OBJECTS = seed.o seed-log.o seed-database.o

EXECUTABLE = aquaOs

all: $(EXECUTABLE)

#note : Write command in order : $(CXX) $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(INCLUDES) $(LIBS) -o $(EXECUTABLE)
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CFLAGS) $(OBJECTS) $(LDFLAGS) $(INCLUDES) $(LIBS) -o $(EXECUTABLE)

# pull in dependency info for *existing* .o files
-include $(OBJECTS:.o=.d)

%.o: %.cc
	$(CXX) -c $(CFLAGS) $(INCLUDES) $*.cc -o $*.o

	$(CXX) -MM $(CFLAGS) $(INCLUDES) $*.cc > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	$(RM) *.o $(EXECUTABLE)