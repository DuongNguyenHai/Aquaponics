#!/bin/bash

# Nguyen Hai Duong
# Date : july 2016

# just ./install.sh

# Build seed :

rm -f *.o seed

# gcc -Wall -c seed-sql.c -o seed-sql.o $(pkg-config --cflags --libs libmongoc-1.0)

# g++ -Wall -c seed.cpp seed-sql.c $(pkg-config --cflags --libs libmongoc-1.0)

# g++ -Wall -o seed seed.o seed-sql.o $(pkg-config --cflags --libs libmongoc-1.0)


arm-linux-gnueabihf-gcc -Wall -c seed-sql.c -o seed-sql.o -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0

arm-linux-gnueabihf-g++ -Wall -c seed.cpp seed-sql.c -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0

arm-linux-gnueabihf-g++ -Wall -o seed2 seed.o seed-sql.o -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0



# g++ -Wall seed.cpp -L/usr/local/lib -I/usr/local/include/libmongoc-1.0 \
# 	-I/usr/local/include/libbson-1.0 -lmongoc-1.0 -lbson-1.0 -o seed 

