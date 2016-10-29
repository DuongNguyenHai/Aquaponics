#!/bin/bash

# Nguyen Hai Duong
# Date : july 2016

# just ./install.sh

# Build seed :
rm -f *.o seed

# gcc -Wall -c seed-database.c -o seed-database.o $(pkg-config --cflags --libs libmongoc-1.0)

# g++ -Wall -c seed.cpp seed-database.c $(pkg-config --cflags --libs libmongoc-1.0)

# g++ -Wall -o seed seed.o seed-database.o $(pkg-config --cflags --libs libmongoc-1.0)

g++ -Wall ../lib/seed-log.cc -c

g++ -Wall ../lib/seed-database.cc -L../lib/base/lib-mongo/lib \
	-I../lib/base/lib-mongo/include/libmongoc-1.0 -I../lib/base/lib-mongo/include/libbson-1.0 \
	-lmongoc-1.0 -lbson-1.0 -c

g++ -Wall seed.cc -L../lib/base/lib-mongo/lib \
	-I../lib/base/lib-mongo/include/libmongoc-1.0 -I../lib/base/lib-mongo/include/libbson-1.0 \
	-lmongoc-1.0 -lbson-1.0 -c

g++ -Wall -o seed seed.o seed-database.o seed-log.o -L../lib/base/lib-mongo/lib \
	-I../lib/base/lib-mongo/include/libmongoc-1.0 -I../lib/base/lib-mongo/include/libbson-1.0 \
	-lmongoc-1.0 -lbson-1.0


