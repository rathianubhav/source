#ifndef __INBUILTS__
#define __INBUILTS__

#include "node.h"
#include "value.h"

typedef vector<Value> Args;

class InBuilt : public Method, public Statment {
protected:
    vector<Identifier*> *inarg;
    string id;
public:
    string get_id() {return id;}
    virtual Statment* get_body() override {return this;}
    vector<Identifier*> *get_args() override {return inarg;}

    InBuilt() {
        inarg = new vector<Identifier*>();
    }

    void set_args(int size) {
        for(int i = 0; i < size; i++) {
            auto a = "__args__" + to_string(i);
            inarg->push_back(new Identifier(a.c_str()));
        }
    }

    virtual int exec(Context& cc) override {
        vector<Value> args;
        for(auto a : *inarg) {
            if (cc.st.isdefined(a->get())) args.push_back(cc.st.lookup(a->get()));
            else args.push_back(Value());
        }
        Value v = run(args);
        cc.st.rebind("ret", v);
        return 0;
    }   

    virtual Value run(vector<Value> args) = 0;
};

class Sleep : public InBuilt {
public:
    Sleep() {
        id = "sleep";
        set_args(1);
    }
    virtual Value run(vector<Value> args) override;
};

class Append : public InBuilt {
public:
    Append() {
        id = "append";
        set_args(2);
    }

    virtual Value run(vector<Value> args) override;
};

class Len : public InBuilt {
public:
    Len() {
        id = "len";
        set_args(1);
    }

    virtual Value run(Args args) override;
};

class IsType : public InBuilt {
private:
    Type t;
public:
    IsType(Type t): t(t) {
        switch (t) {
            case INT_T: id = "is_int"; break;
            case FLOAT_T: id = "is_float"; break;
            case STRING_T: id = "is_str"; break;
            case BOOL_T: id = "is_bool"; break;
            case FUNCTION_T: id = "is_func"; break;
            case ARRAY_T: id = "is_arr"; break;
            case ANY_T: id = "is_any"; break;
            default:
                cout << "Error: invalid datatype specified " << t << endl;
                exit(6);
        }

        set_args(1);
    }
    virtual Value run(Args args) override;
};


#endif