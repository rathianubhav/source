#ifndef __VALUE__
#define __VALUE__

#include <string>
#include <iostream>
#include <utils.h>
#include <vector>
#include <map>

using namespace std;

class Method;
class SymbolTable;
class Statment;
class Identifier;
class Expression;
class Value;


enum Type {
    INT_T, FLOAT_T, BOOL_T, NONE_T, STRING_T, ANY_T,
    FUNCTION_T, ARRAY_T, CONTAINER_T,
};

typedef pair<Identifier*, Expression*> ContainerData;


class ContainerDef {
public:
    virtual Value get_val(const string& id) = 0;
};

class ContainerClosure {
public:
    ContainerDef *container_def;
    SymbolTable* environment;
};

class MethodDef {
public:
    virtual vector<Identifier*> *get_args() = 0;
    virtual Statment* get_body() = 0;
};

class Closure {
public:
    MethodDef* method_def;
    SymbolTable* environment;
};


class Value {
private:
    union {
        int Integer;
        float Float;
        bool Bool;
        string* String;
        void* Any;
        Closure Function;
        ContainerClosure Container;
        vector<Expression*> *Array;
    } value;

    Type type;

public:

    string get_type_str() {
        switch(type) {
            case INT_T: return "int";
            case FLOAT_T: return "float";
            case STRING_T: return "string";
            case BOOL_T: return "bool";
            case ANY_T: return "any";
            case FUNCTION_T: return "method";
            case ARRAY_T: return "array";
            case CONTAINER_T: return "container";
        }
        return "unknown";
    }
    static string print_type(Type t) {
        switch(t) {
            case INT_T: return "int";
            case FLOAT_T: return "float";
            case STRING_T: return "string";
            case BOOL_T: return "bool";
            case ANY_T: return "any";
            case FUNCTION_T: return "method";
            case ARRAY_T: return "array";
            case CONTAINER_T: return "container";
        }
        return "unknown";
    }
    static void check_type(Value v, Type t) {
        if (v.get_type() != t) {
            cout << "Error: illegal datatype " << v.get_type_str() << ", but required '" << Value::print_type(t) << "'" << endl;
            exit(5); 
        }
    }

    explicit Value() { type=NONE_T;}
    explicit Value(int n) { type=INT_T; value.Integer = n;}
    explicit Value(float f) { type=FLOAT_T; value.Float = f;}
    explicit Value(bool b) { type=BOOL_T; value.Bool = b;}
    explicit Value(const string& s) { type=STRING_T; value.String = new string(s);}
    explicit Value(void* v) { type=ANY_T; value.Any = v;}
    explicit Value(vector<Expression*> *arr) { type = ARRAY_T; value.Array = arr;}
    explicit Value(MethodDef* f, SymbolTable* env)
    {
        type = FUNCTION_T;
        value.Function.method_def = f;
        value.Function.environment = env;
    }

    explicit Value(ContainerDef* c, SymbolTable* env)
    {
        type = CONTAINER_T;
        value.Container.container_def = c;
        value.Container.environment = env;
    }

    Type get_type() {return type;}
    void set_type(Type t) {type = t;}
    bool oftype(Type t) {
        return type == t;
    }

    int Int() {return value.Integer;}
    float Float() {return value.Float;}
    bool Bool() {return value.Bool;}
    string String() {return *value.String;}
    void* Any() {return value.Any;}
    vector<Expression*> *Array() {return value.Array;}
    Closure Function() {return value.Function;}
    ContainerClosure Container() {return value.Container;}



    void repr(ostream &out) {
        switch(type) {
            case INT_T: out << value.Integer; break;
            case FLOAT_T: out << value.Float; break;
            case BOOL_T: out << value.Bool ? "true" : "false"; break;
            case STRING_T: out << *value.String; break;
            case NONE_T: out << "unset"; break;
            case ANY_T: out << value.Any; break;
            case ARRAY_T: out << "array"; break;
            case FUNCTION_T: out << "function"; break;
            case CONTAINER_T: out << "container"; break;
            default: out << "illegal_variable"; break;
        }
    }

    bool operator==(const Value& other) {
        if (type != other.type) {
            return false;
        }

        switch(type) {
            case INT_T: return value.Integer == other.value.Integer;
            case FLOAT_T: return value.Float == other.value.Float;
            case BOOL_T: return value.Bool == other.value.Bool;
            case STRING_T: return *value.String == *other.value.String;
            case NONE_T: return true;
            case FUNCTION_T: return false;
            case ARRAY_T: {
                return equal(value.Array->begin(), value.Array->end(),
                      other.value.Array->begin());
            }
            case ANY_T: return value.Any == other.value.Any;
        }
        return false;
    }

    bool operator<(const Value& other) {
        if (type != other.type) return false;
        switch(type) {
            case INT_T: return value.Integer < other.value.Integer;
            case FLOAT_T: return value.Float < other.value.Float;
            case BOOL_T: return value.Bool < other.value.Bool;
            case STRING_T: return value.String->length() < other.value.String->length();
            case NONE_T: return false;
            case FUNCTION_T: return false;
            case ARRAY_T: return value.Array->size() < other.value.Array->size();
            case ANY_T: return false;
        }

        return false;
    }

    Value operator+(const Value& other) {
        switch(type) {
            case INT_T: {
                switch (other.type) {
                    case INT_T: return Value(value.Integer + other.value.Integer);
                    case FLOAT_T: return Value(value.Integer + (int)other.value.Float);
                    case STRING_T: {
                        if (isInt(*other.value.String))
                            return Value(value.Integer + atoi(other.value.String->c_str()));
                        else {
                            cout << "Error: can't add integer with string" << endl;
                            exit(5);
                        }
                    }
                    default:
                        cout << "can't add int with " + Value::print_type(other.type) << endl;
                        exit(6);
                }
            }

            case FLOAT_T: {
                switch (other.type) {
                    case INT_T: return Value(value.Float + (float)other.value.Integer);
                    case FLOAT_T: return Value(value.Float + other.value.Float);
                    case STRING_T: {
                        if (isFloat(*other.value.String))
                            return Value(value.Float + strtof(other.value.String->c_str(),0));
                        else {
                            cout << "Error: can't add float with string" << endl;
                            exit(5);
                        }
                    }
                    default:
                        cout << "can't add float with " + Value::print_type(other.type) << endl;
                        exit(6);
                }
            }

            case STRING_T: {
                switch (other.type) {
                    case INT_T: return Value(value.String->append(to_string(other.value.Integer)));
                    case FLOAT_T: return Value(value.String->append(to_string(other.value.Float)));
                    case STRING_T: return Value(value.String->append(*other.value.String));
                    default:
                        cout << "can't add string with " + Value::print_type(other.type) << endl;
                        exit(6);
                }
            }
        }

        cout << "illegal operation between" << Value::print_type(type) << " and " << Value::print_type(other.type) << endl;
        exit(6);

        return Value();
    }

    Value operator-(const Value& other) {
        switch(type) {
            case INT_T: {
                switch (other.type) {
                    case INT_T: return Value(value.Integer - other.value.Integer);
                    case FLOAT_T: return Value(value.Integer - (int)other.value.Float);
                    default:
                        cout << "can't subtract int with " + Value::print_type(other.type) << endl;
                        exit(6);
                }
            }

            case FLOAT_T: {
                switch (other.type) {
                    case INT_T: return Value(value.Float - (float)other.value.Integer);
                    case FLOAT_T: return Value(value.Float - other.value.Float);
                    default:
                        cout << "can't subtract float with " + Value::print_type(other.type) << endl;
                        exit(6);
                }
            }
        }
        cout << "illegal operation between" << Value::print_type(type) << " and " << Value::print_type(other.type) << endl;
        exit(6);

        return Value();
    }

    Value operator*(const Value& other) {
        switch(type) {
            case INT_T: {
                switch (other.type) {
                    case INT_T: return Value(value.Integer * other.value.Integer);
                    case FLOAT_T: return Value(value.Integer * (int)other.value.Float);
                    default:
                        cout << "can't multiply int with " + Value::print_type(other.type) << endl;
                        exit(6);
                }
            }

            case FLOAT_T: {
                switch (other.type) {
                    case INT_T: return Value(value.Float * (float)other.value.Integer);
                    case FLOAT_T: return Value(value.Float * other.value.Float);
                    default:
                        cout << "can't multiply float with " + Value::print_type(other.type) << endl;
                        exit(6);
                }
            }
        }
        cout << "illegal operation between" << Value::print_type(type) << " and " << Value::print_type(other.type) << endl;
        exit(6);

        return Value();
    }

    Value operator/(const Value& other) {
        switch(type) {
            case INT_T: {
                switch (other.type) {
                    case INT_T: return Value(value.Integer / other.value.Integer);
                    case FLOAT_T: return Value(value.Integer / (int)other.value.Float);
                    default:
                        cout << "can't divide int with " + Value::print_type(other.type) << endl;
                        exit(6);
                }
            }

            case FLOAT_T: {
                switch (other.type) {
                    case INT_T: return Value(value.Float / (float)other.value.Integer);
                    case FLOAT_T: return Value(value.Float / other.value.Float);
                    default:
                        cout << "can't divide float with " + Value::print_type(other.type) << endl;
                        exit(6);
                }
            }
        }
        cout << "illegal operation between" << Value::print_type(type) << " and " << Value::print_type(other.type) << endl;
        exit(6);

        return Value();
    }


};

#endif