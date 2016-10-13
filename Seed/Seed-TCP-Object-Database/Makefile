
CC = gcc
CXX = g++

CFLAGS = -Wall -g

LDFLAGS = -L../lib/base/lib-mongo/lib -Wl,-rpath=../lib/base/lib-mongo/lib

LIBS = -lmongoc-1.0 -lbson-1.0

INCLUDES = -I../lib/base/lib-mongo/include/libmongoc-1.0 -I../lib/base/lib-mongo/include/libbson-1.0

# source file
SOURCES = ../lib/seed-log.cc ../lib/seed-database.cc ../lib/seed-object.cc ../lib/TCP-IP.cc seed.cc

# convert name.c to name.o and name.cc to name.o
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