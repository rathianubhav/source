#!/bin/sh

rm ./build/source
echo "compiling libsrc"
g++ src/libsrc/*.cc -I include -fPIC -shared -o build/libsrc -std=c++17

echo "compiling source"
g++ src/source/*.cc -I include -o build/source -std=c++17 -Lbuild/ -lsrc

echo "Compilation done"
LD_LIBRARY_PATH=./build/ ./build/source