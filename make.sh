#!/bin/sh
rm build/source
cd src

echo "compiling lexer"
flex -o lex.yy.cc lex.l

echo "compiling parser"
bison -d parser.y

echo "building source"
g++ -std=c++14 `llvm-config --cxxflags --ldflags --system-libs --libs core` -lrbase -Wno-write-strings -fpermissive  -ggdb -O2 *.cc *.c -I ../include -o ../build/source

rm *.tab.* *.yy.*
