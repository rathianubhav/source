#pragma once

#include "node.h"
#include "value.h"

class InBuilt : public source::MethodDefination, public Statment {
protected:
    std::vector<Identifier*> *inargs;
    std::string id;
    int size = 0;
public:
    std::string getID() {return id;}
    virtual Statment* get_body() override {return this;}
    std::vector<Identifier*> *get_args() override {return inargs;}

    virtual void exec(context::Context* cc) override {
        for(int i = 0; i <= 10; i++) {
            auto a = "__args__" + std::to_string(i);
            inargs->push_back(new Identifier(a.c_str()));
        }

        std::vector<Value> args;
        for(auto a : *inargs) {
            if (cc->st->defined(a->get())) {
                args.push_back(cc->st->lookup(a->get()));
                size++;
            } else {
                args.push_back(Value("__undefine__"));
            }
            
        }

        Value result = run(args);
        cc->st->rebind("ret", result);
    }


    virtual Value run(std::vector<Value> args) = 0;

};


class Typeof : public InBuilt {
public:
    Typeof() {
        id = "typeof";
    }

    virtual Value run(std::vector<Value> args) override;
};