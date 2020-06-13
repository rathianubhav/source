#include <inbuilts.h>

Value
Typeof::run(std::vector<Value> args)
{
    for(int i = 0; i < size; i++) {
        switch (args.at(i).getType()) {
            case INT_T: return Value("int");
            case FLOAT_T: return Value("float");
            case STR_T: return Value("string");
            case BOOL_T: return Value("bool");
            case NONE_T: return Value("null");
            case ANY_T: return Value("any");
            case FUNC_T: return Value("func");
            case ARRAY_T: return Value("array");
            case DICT_T: return Value("container");
        }
    }

    return Value("null");
}