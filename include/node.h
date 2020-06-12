#pragma once

#include "buffer.h"
#include "context.h"
#include <vector>

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

class Expression : public Node {
public:
    virtual Value eval(context::Context* cc) {
        err << "err[node]: eval not yet implemented in " << typeid(*this).name() << std::endl;
        return Value();
    }
};

class Identifier : public Expression {
private:
    std::string value;
public:
    Identifier(const char* v) : value(v) {}
    std::string& get() {return value;}

    virtual Value eval(context::Context* cc) override;
};

class Number : public Expression {
private:
    std::string value;
public:
    Number(const char* val) : value(val) {}
    virtual Value eval(context::Context* cc) override;

};

class Bool : public Expression {
private:
    bool value;
public:
    Bool(bool val) : value(val) {}
    virtual Value eval(context::Context* cc) override;

};

class String : public Expression {
private:
    std::string value;
public:
    String(const char* s) : value(s) {}
    virtual Value eval(context::Context* cc) override;
};


class Array : public Expression {
private:
    std::vector<Expression*> *exprs;
public:
    Array(std::vector<Expression*> *exprs) : exprs(exprs) {}
    void respr(context::Context* cc, std::ostream& out);
    virtual Value eval(context::Context* cc) override;
};


class Null : public Expression {
private:
public:
    Null() {}
    virtual Value eval(context::Context* cc) override {
        return Value();
    }
};

class Access : public Expression {
private:
    Identifier* id;
    Expression* expr;
public:
    Access(Identifier* id,Expression* expr) : id(id), expr(expr) {}
    virtual Value eval(context::Context* cc) override;
};



class Arithmetic : public Expression {
private:
    Oper op;
    Expression *left, *right; 
public:
    Arithmetic(Expression*left, Oper op, Expression* right)
    :   left(left), op(op), right(right) {}

    virtual Value eval(context::Context* cc) override;

    virtual ~Arithmetic() { delete left; delete right;}
};

class Compare : public Expression {
private:
    Oper op;
    Expression *left, *right; 
public:
    Compare(Expression*left, Oper op, Expression* right)
    :   left(left), op(op), right(right) {}

    virtual Value eval(context::Context* cc) override;

    virtual ~Compare() { delete left; delete right;}
};


class Typeof : public Expression {
private:
    Expression* expr;
public:
    Typeof(Expression* expr) : expr(expr) {}
    virtual Value eval(context::Context* cc) override;
    virtual ~Typeof() { delete expr;}
};


class Statment : public Node {
private:
public:
    Statment() {}
    virtual void exec(context::Context *cc) {
        err << "err[node] exec() not yet implemented for " << typeid(*this).name() << std::endl; 
    }
};

class Block : public Statment {
private:
    std::vector<Statment*> *body;
public:
    Block(std::vector<Statment*> *body) : body(body) {}

    virtual void exec(context::Context* cc) override;
    virtual ~Block() {
        for(auto b : *body) {
            delete b;
        }
        body->clear();
    }
};


class Condition : public Statment {
private:
    Expression* expr;
    Statment* ifblock, *elseBlock;

public:
    Condition(Expression* expr, Statment* ib, Statment* eb = nullptr)
    : expr(expr), ifblock(ib), elseBlock(eb) {}

    virtual void exec(context::Context *cc) override;
    virtual ~Condition() {
        delete expr;
        delete ifblock;
        if (elseBlock) delete elseBlock;
    }
};


class Loop : public Statment {
private:
    Expression* expr;
    Statment* body;

public:
    Loop(Expression* expr, Statment* body)
        : expr(expr), body(body) {}

    virtual void exec(context::Context* cc) override;
    virtual ~Loop() {
        delete expr;
        delete body;
    }
};


class Let : public Statment {
private:
    Identifier* id;
    Expression* expr;

public:
    Let(Identifier* id, Expression* expr)
        : id(id), expr(expr) {}

    virtual void exec(context::Context* cc) override;

    virtual ~Let() {
        delete id;
        delete expr;
    }
};


class Assign : public Statment {
private:
    Identifier* id;
    Expression* expr;

public:
    Assign(Identifier* id, Expression* expr)
        : id(id), expr(expr) {}

    virtual void exec(context::Context* cc) override;

    virtual ~Assign() {
        delete id;
        delete expr;
    }
};

class Print : public Statment {
private:
    std::vector<Expression*> *expr;
public:
    Print(std::vector<Expression*> *expr) : expr(expr) {}

    virtual void exec(context::Context* cc) override;
    virtual ~Print() {
        for(auto a : *expr) {
            delete a;
        }
        expr->clear();
    }
};


class Println : public Statment {
private:
    std::vector<Expression*> *expr;
public:
    Println(std::vector<Expression*> *expr) : expr(expr) {}

    virtual void exec(context::Context* cc) override;
    virtual ~Println() {
        for(auto a : *expr) {
            delete a;
        }
        expr->clear();
    }
};

class ExpressionStatment : public Statment {
private:
    Expression* body;
public:
    ExpressionStatment(Expression* body) : body(body) {}

    virtual void exec(context::Context* cc) override;
    virtual ~ExpressionStatment() { delete body;}
};

class Method : public source::MethodDefination, public Expression {
private:
    std::vector<Identifier*> *id;
    Statment* body;
public:
    Method(std::vector<Identifier*> *id, Statment* body)
    : id(id), body(body) {}

    std::vector<Identifier*> *get_args() {return id;}
    Statment* get_body() {return body;}

    virtual Value eval(context::Context* cc) override;
    virtual ~Method() {
        delete id;
        delete body;
    }
};

class Call : public Expression {
private:
    std::vector<Expression*> *arg;
    Identifier *id;
public:
    Call(Identifier* id, std::vector<Expression*> *a) : id(id), arg(a) {}
    virtual Value eval(context::Context* cc) override;

    virtual ~Call() {
        delete id;
        for(auto a : *arg) {
            delete a;
        }
        arg->clear();
    }
};


class Use : public Statment {
private:
    String* modname;
public:
    Use(String* md) : modname(md) {}

    virtual void exec(context::Context* cc) override;
    virtual ~Use() {
        delete modname;
    }
};