#!/bin/sh
rm build/source
cd src

echo "compiling lexer"
flex -o lex.yy.cc lex.l

echo "compiling parser"
bison -d parser.y

echo "building source"
g++ -std=c++11 -Wno-write-strings -fpermissive -lrbase  -ggdb -fmax-errors=3 -O2 *.cc *.c -I ../include -o ../build/source

rm *.tab.* *.yy.*
