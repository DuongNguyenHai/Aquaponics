#!/bin/bash

# Nguyen Hai Duong
# Date : july 2016

# just ./install.sh

# Build seed :
rm -f *.o seed

g++ -Wall ../lib/seed-log.cc -c

g++ -Wall ../lib/seed-database.cc -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0 -c

g++ -Wall ../lib/seed-object.cc -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0 -c

g++ -Wall seed.cc -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0 -c

g++ -Wall seed.o seed-database.o seed-object.o seed-log.o -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0 -o seed


