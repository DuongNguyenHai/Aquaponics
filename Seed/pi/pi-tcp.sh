#!/bin/bash

# Nguyen Hai Duong
# Date : july 2016

# Build cross platform raspberry Pi
# just ./pi.sh

# Build seed :
rm -f seed

arm-linux-gnueabihf-g++ -std=c++11 -Wall seed.cpp TCP-IP.cpp cmd-config.cpp seed-log.cpp -o seed 
