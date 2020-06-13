#pragma once

#include <cstring>
#include <iostream>
#include <vector>

class Identifier;
class Statment;
class Expression;

class Method;

namespace source {


class ST;

enum Type { INT_T, FLOAT_T, BOOL_T, NONE_T, STR_T, ANY_T, FUNC_T, ARRAY_T, DICT_T};


typedef std::pair<Identifier*, Expression*> dict;

class MethodDefination{
public:
    virtual std::vector<Identifier*> *get_args() = 0;
    virtual Statment* get_body() = 0;
};

class Closure {
public:
    Method* func;
    ST* env;
};

class Value {
private:
    union {
        int Int;
        double Float;
        bool Bool;
        char* Str;
        void* Any;
        Closure Func;
        std::vector<Expression*> *Arr;
        std::vector<dict*> *Dict;
    } value;

    Type type;
public:

    explicit Value()        { type = NONE_T; }
    explicit Value(int n)   { type = INT_T;  value.Int  = n;}
    explicit Value(double d){ type = FLOAT_T; value.Float = d;}
    explicit Value(bool b)  { type = BOOL_T; value.Bool = b;}
    explicit Value(char* c) { type = STR_T;  value.Str  = c;}
    explicit Value(void* v) { type = ANY_T;  value.Any  = v;}
    explicit Value(std::vector<Expression*> *arr) { type = ARRAY_T; value.Arr = arr;}
    explicit Value(Method* f, source::ST* env) { 
        type = FUNC_T;
        value.Func.func =f;
        value.Func.env = env;

    }


    explicit Value(std::vector<dict*> *d) { type = DICT_T, value.Dict = d;}


    Type getType() {return type;}
    void setType(Type t) {type = t;}

    int Int() {return value.Int;}
    double Float() {return value.Float;}
    bool Bool() {return value.Bool;}
    char* Str() {return value.Str;}
    void* Any() {return value.Any;}
    std::vector<Expression*> *Arr() {return value.Arr;}
    Closure Func() {return value.Func;}
    std::vector<dict*> *Dict() {return value.Dict;}

    void repr(std::ostream& out) {
        switch(type) {
            case INT_T: out << value.Int; break;
            case FLOAT_T: out << value.Float; break;
            case BOOL_T: out << (value.Bool ? "true": "false"); break;
            case STR_T: out << value.Str; break;
            case NONE_T: out << "unset"; break;
            case ANY_T: out << value.Any; break;
        }
    }

    bool operator==(const Value& other) {
        if (type != other.type) {
            return false;
        }
        switch(type) {
            case INT_T: return value.Int == other.value.Int;
            case FLOAT_T: return value.Float == other.value.Float;
            case BOOL_T: return value.Bool == other.value.Bool;
            case STR_T: return !strcmp(value.Str, other.value.Str);
            case FUNC_T: return false;
            case NONE_T: return true;
            case ANY_T: return value.Any == other.value.Any;
        }
        return false;
    }

    bool operator<(const Value& other) {
        if (type != other.type) return false;
        switch(type) {
            case INT_T: return value.Int < other.value.Int;
            case FLOAT_T: return value.Float < other.value.Float;
            case BOOL_T: return value.Bool < other.value.Bool;
            case STR_T: return strlen(value.Str) <  strlen(other.value.Str);
            case FUNC_T: return false;
            case NONE_T: return false;
            case ANY_T: return value.Any < other.value.Any;
        }
        return false;
    }

    Value operator+(const Value& other) {
        if (type != other.type) return Value();
        switch(type) {
            case INT_T: return Value(value.Int + other.value.Int);
            case FLOAT_T: return Value(value.Float + other.value.Float);
            case BOOL_T: return Value(value.Bool + other.value.Bool);
            case STR_T: {
                std::string a(value.Str);
                std::string r(other.value.Str);
                return Value((char*)(a + r).c_str());
            }
        }
        return Value();
    }

    Value operator-(const Value& other) {
        if (type != other.type) return Value();
        switch(type) {
            case INT_T: return Value(value.Int - other.value.Int);
            case FLOAT_T: return Value(value.Float - other.value.Float);
        }
        return Value();
    }

    Value operator*(const Value& other) {
        if (type != other.type) return Value();
        switch(type) {
            case INT_T: return Value(value.Int * other.value.Int);
            case FLOAT_T: return Value(value.Float * other.value.Float);
        }
        return Value();
    }

    Value operator/(const Value& other) {
        if (type != other.type) return Value();
        switch(type) {
            case INT_T: return Value(value.Int / other.value.Int);
            case FLOAT_T: return Value(value.Float / other.value.Float);
        }
        return Value();
    }


};

}