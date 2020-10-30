#!/bin/sh

[[ -e build/source ]] && rm ./build/source
echo "compiling libsrc"
g++ src/libsrc/*.cc -I include -fPIC -shared -o build/libsrc.so -std=c++17 -fpermissive -Wno-int-to-pointer-cast -Wno-return-type

echo "compiling source"
g++ src/source/*.cc -I include -o build/source -std=c++17 -Lbuild/ -lsrc

echo "Compilation done"
LD_LIBRARY_PATH=./build/ ./build/source