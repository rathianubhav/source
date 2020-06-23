#pragma once

#include "buffer.h"
#include "context.h"
#include <vector>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>

extern source::out::stream err;

using namespace source;
enum Oper { ADD, SUB, MUL, DIV, LT, GT, LE, GE, AND, OR, NOT, EQ, NE};

class Node {
private:
protected:

public:
    Node() {}
    virtual ~Node() {}

};

class DataType : public Node {
public:
    virtual llvm::Type *gen(context::Context& cc) {
        std::cout << "datatype not yet implemented for " << typeid(*this).name() << std::endl;
        return nullptr;
    }
};

class IntType : public DataType {
private:
    int size;
    bool is_signed;
public:
    IntType(int size = 0, bool is_signed = true)
    : size(size), is_signed(is_signed) {}

    virtual llvm::Type *gen(context::Context& cc) override;

};


class Expression : public Node {
public:
    virtual llvm::Value* codegen(context::Context &cc) {
        err << "codegen() not yet implemented for " << typeid(*this).name() << std::endl;
        return nullptr;
    }
};

class Identifier : public Expression {
private:
    std::string value;
        
public:
    Identifier(const char* v) : value(v) {}
    std::string& get() {return value;}
    virtual llvm::Value* codegen(context::Context& cc) override;
    
};

class Number : public Expression {
private:
    std::string value;
public:
    Number(const char* val) : value(val) {}
    virtual llvm::Value* codegen(context::Context& cc) override;

};

class Arithmetic : public Expression {
private:
    Oper op;
    Expression *left, *right; 
public:
    Arithmetic(Expression*left, Oper op, Expression* right)
    :   left(left), op(op), right(right) {}

    virtual llvm::Value* codegen(context::Context& cc) override;
    virtual ~Arithmetic() { delete left; delete right;}
};


class Statment : public Node {
public:
    virtual llvm::Value* codegen(context::Context& cc) {
        err << "codegen() not yet implemented for statment " << typeid(*this).name() << std::endl;
        return nullptr;
    }
};


class Argument : public Statment {
public:
    Identifier *name;
    DataType* t;
    Argument(Identifier*n, DataType* t)
    : name(n), t(t) {}
    virtual llvm::Value* codegen(context::Context& cc) override {}
};

class Block : public Statment {
private:
    std::vector<Statment*> *body;
public:
    Block(std::vector<Statment*> *body) : body(body) {}

    virtual llvm::Value* codegen(context::Context& cc) override;
};


class TopLevel : public Node {
public:
    virtual llvm::Function* codegen(context::Context& cc) {
        err << "codegen() not yet implemented for " << typeid(*this).name() << std::endl;
        return nullptr;
    }
};

class Prototype : public TopLevel {
private:
    std::vector<Argument*> *args;
    Identifier* id;
    DataType* rettype;
public:
    Prototype(Identifier* id, std::vector<Statment*> *a, DataType* rettype)
    : id(id), rettype(rettype) {
        args = new std::vector<Argument*>();
        for(auto i : *a ) args->push_back((Argument*)i);
        delete a;
    }
    std::string getid() {return id->get();}
    virtual llvm::Function* codegen(context::Context& cc) override;
};

class Method : public TopLevel {
private:
    Prototype* proto;
    Block* body;
public:
    Method() {}
    Method(Prototype* proto, Block* body)
    : proto(proto),
      body(body) {}

    virtual llvm::Function* codegen(context::Context& cc) override;
};

class Return : public Statment {
private:
    Expression *expr;
public:
    Return(Expression* expr) : expr(expr) {}
    virtual llvm::Value* codegen(context::Context& cc) override;
};

class Call : public Statment {
private:
    std::vector<Expression*> *arg;
    Identifier *id;
public:
    Call(Identifier* id, std::vector<Expression*> *a) : id(id), arg(a) {}
    virtual llvm::Value* codegen(context::Context& cc) override;

    virtual ~Call() {
        delete id;
        for(auto a : *arg) {
            delete a;
        }
        arg->clear();
    }
};


class Let : public Statment {
private:
    Identifier* id;
    DataType *t;
    Expression* expr;
public:
    Let(Identifier* id, DataType* t, Expression *expr = nullptr)
    : id(id), t(t), expr(expr) {}

    virtual llvm::Value* codegen(context::Context &cc) override;

    virtual ~Let() {
        delete id, t, expr;
    }
};