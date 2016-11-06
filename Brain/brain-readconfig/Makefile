CC = gcc
CXX = g++

CFLAGS += -std=c++11 -Wall -g 
LDFLAGS +=

# source file
SOURCES = ../lib/seed-parse-config.cc ../lib/seed-log.cc ../lib/seed-config.cc main.cc
INCLUDES = -I../lib
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
	$(CXX) -c $(CFLAGS) $(LDFLAGS) $(INCLUDES) $*.cc -o $*.o

	$(CXX) -MM $(CFLAGS) $(LDFLAGS) $(INCLUDES) $*.cc > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	$(RM) *.o $(EXECUTABLE)

#note : viet theo thu tu : *.o $(INCLUDES) $(LDFLAGS) $(LIBS) -o $(EXECUTABLE)