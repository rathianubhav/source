#!/bin/sh

rm build/source

cd src
echo "compiling lexer"
flex -o lexer.yy.cc lexer.l

echo "compiling parser"
bison -d parser.y

echo "building source"
g++ -std=c++11 -Wno-write-strings -fpermissive -lrbase -ldl  -L ../build -lsource -ggdb -fmax-errors=3 -O2 *.cc *.c -I ../include -o ../build/source

rm *.tab.* *.yy.*