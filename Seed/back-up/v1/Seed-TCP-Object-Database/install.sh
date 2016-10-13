#!/bin/bash

# Nguyen Hai Duong
# Date : july 2016

# just ./install.sh

# Build seed :

rm -f *.o seed

# gcc -Wall -c seed-database.c -o seed-database.o $(pkg-config --cflags --libs libmongoc-1.0)

# g++ -Wall -c seed.cpp seed-database.c $(pkg-config --cflags --libs libmongoc-1.0)

# g++ -Wall -o seed seed.o seed-database.o $(pkg-config --cflags --libs libmongoc-1.0)

g++ -Wall ../lib/seed-log.cpp -c

g++ -Wall ../lib/seed-database.c -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0 -c

g++ -Wall ../lib/seed-object.cpp -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0 -c

g++ -Wall ../lib/TCP-IP.cpp -c

g++ -Wall seed.cpp -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0 -c

g++ -Wall seed.o seed-database.o seed-object.o seed-log.o TCP-IP.o -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0 -o seed

