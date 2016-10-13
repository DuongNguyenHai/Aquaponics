#!/bin/bash

# Nguyen Hai Duong
# Date : july 2016

# just ./install.sh

# Build seed :
rm -f *.o seed

g++ -Wall ../lib/seed-log.cpp -c 
g++ -Wall ../lib/TCP-IP.cpp -c
g++ -Wall ../lib/cmd-config.cpp -c
g++ -Wall seed.cpp -c

g++ -Wall seed.o seed-log.o TCP-IP.o cmd-config.o -o seed 
