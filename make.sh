#!/bin/sh
rm build/source
cd src

echo "compiling lexer"
flex -o source.yy.cpp source.lpp 

echo "compiling parser"
bison -d source.ypp

echo "building source"
g++ -std=c++11 -Wno-reorder  -Wno-sign-compare -Wno-unused-parameter -fpermissive -Wno-write-strings -lrbase -ldl -ggdb -fmax-errors=3 -O2 *.cc *.cpp -I ../include -o ../build/source

rm *.tab.* *.yy.*
