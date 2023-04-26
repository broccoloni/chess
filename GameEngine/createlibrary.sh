#!/bin/sh

g++ -Wall -c *.cpp
ar rcs libengine.a *.o
rm *.o






