#pragma once

#include "node.h"
#include "value.h"

class InBuilt : public Method, public Statment {
protected:
    std::vector<Identifier*> *inargs;
    std::string id;
public:
    std::string getID() {return id;}
    virtual Statment* get_body() override {return this;}
    std::vector<Identifier*> *get_args() override {return inargs;}

    InBuilt() {
        inargs = new std::vector<Identifier*>();
    }
    void set_args_count(int size) {
        for(int i = 0; i < size; i++) {
            auto a = "__args__" + std::to_string(i);
            inargs->push_back(new Identifier(a.c_str()));
        }
    }
    virtual int exec(context::Context& cc) override {

        std::vector<Value> args;
        for(auto a : *inargs) {
            if (cc.st.defined(a->get())) {
                args.push_back(cc.st.lookup(a->get()));
            } else {
                args.push_back(Value("__undefine__"));
            }
            
        }

        auto r = run(args);
        cc.st.rebind("ret",r);

        return 0;
    }


    virtual Value run(std::vector<Value> args) = 0;

};

typedef InBuilt* create_inbuilt();
typedef void destroy_inbuilt(InBuilt*);


class Typeof : public InBuilt {
public:
    Typeof() {
        id = "typeof";
        set_args_count(1);
    }

    virtual Value run(std::vector<Value> args) override;
};

class Sleep : public InBuilt {
public:
    Sleep() {
        id = "sleep";
        set_args_count(1);
    }

    virtual Value run(std::vector<Value> args) override;
};

class System : public InBuilt {
public:
    System() : InBuilt() {
        id = "system";
        set_args_count(1);
    }

    virtual Value run(std::vector<Value> args) override;
};


class Fopen : public InBuilt {
public:
    Fopen() : InBuilt() {
        id = "fopen";
        set_args_count(2);
    }

    virtual Value run(std::vector<Value> args) override;
};


class Fgetc : public InBuilt {
public:
    Fgetc() {
        id = "fgetc";
        set_args_count(1);
    }

    virtual Value run(std::vector<Value> args) override;
};

class Fputc : public InBuilt {
public:
    Fputc() {
        id = "fputc";
        set_args_count(2);
    }

    virtual Value run(std::vector<Value> args) override;
};

class Fclose : public InBuilt {
public:
    Fclose() : InBuilt() {
        id = "fclose";
        set_args_count(1);
    }

    virtual Value run(std::vector<Value> args) override;
};


class Range : public InBuilt {
public:
    Range() {
        id = "range";
        set_args_count(1);
    }

    virtual Value run(std::vector<Value> args) override;
};

class Append : public InBuilt {
public:
    Append() {
        id = "append";
        set_args_count(2);
    }

    virtual Value run(std::vector<Value> args) override;
};

class CLib : public InBuilt {
public:
    CLib() {
        id = "clib";
        set_args_count(2);
    }

    virtual Value run(std::vector<Value> args) override;
};

