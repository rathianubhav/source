#!/bin/sh

rm ./source
echo "compiling source"
g++ src/*.cc -I include -o source -std=c++17 -DDEBUG


echo "Compilation done"
./source