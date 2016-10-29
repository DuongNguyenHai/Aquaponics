CC = gcc
CXX = g++

# CFLAGS = -Wall -g
# LDFLAGS = -L../lib/base/lib-mongo/lib -Wl,-rpath=../lib/base/lib-mongo/lib
# LIBS = -lmongoc-1.0 -lbson-1.0

CFLAGS += -std=c++11 -Wall -g `pkg-config --cflags --libs libmongoc-1.0`
LDFLAGS += `pkg-config --cflags --libs libmongoc-1.0`

# INCLUDES = -I../lib/base/lib-mongo/include/libmongoc-1.0 -I../lib/base/lib-mongo/include/libbson-1.0

# source file
SOURCES = ../lib/seed-log.cc ../lib/seed-database.cc main.cc

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
	$(CXX) -c $(CFLAGS) $(LDFLAGS) $*.cc -o $*.o

	$(CXX) -MM $(CFLAGS) $(LDFLAGS) $*.cc > $*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp

clean:
	$(RM) *.o $(EXECUTABLE)




#note : viet theo thu tu : *.o $(INCLUDES) $(LDFLAGS) $(LIBS) -o $(EXECUTABLE)