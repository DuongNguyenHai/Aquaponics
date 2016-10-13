#!/bin/bash

# Nguyen Hai Duong
# Date : july 2016

# just ./install.sh

# Build seed :
rm -f *.o seed

g++ -Wall ../lib/seed-log.cc -c 
g++ -Wall ../lib/TCP-IP.cc -c
g++ -Wall ../lib/cmd-config.cc -c
g++ -Wall seed.cc -c

g++ -Wall seed.o seed-log.o TCP-IP.o cmd-config.o -o seed 
