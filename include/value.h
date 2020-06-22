#pragma once

#include <cstring>
#include <iostream>
#include <vector>
#include <math.h>
#include <llvm/IR/Value.h>

namespace source {


enum Type { INT_T, FLOAT_T, BOOL_T, NONE_T, STR_T, ANY_T, FUNC_T, ARRAY_T, DICT_T, POINT_T};

class Value {
private:
    llvm::Value* value;

    Type type;
    bool Constant = false;
public:

    explicit Value(llvm::Value *val) : value(val) {}
    explicit Value() : value(nullptr) {}
    llvm::Value *get() { return value;}
    Type gettype() { return type;}
    bool isConstant() { return Constant;}
};

}