#ifndef __AST__
#define __AST__

#include <string>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <typeinfo>
#include <compiler.hh>
#include <frame.hh>


enum Oper {
    ADD, SUB,
    MUL, DIV,
    LT, GT, LE, GE,
    EQ, NE,
    AND, OR, NOT,
};

class Node {

protected:
    vector<Node*> child;

    void add(Ast* c) { child.push_back(c);}

public:
    string label = "(node)";
    Node() {}
    virtual ~Node() {
        for(auto c : child) delete c;
        child.clear();
    }

};

class Expression : public Node {
public:
    string label = "(expression)";
    virtual string eval(Frame* st, Context* con) {
        errout << "err[ast]: eval not implemented for " << typeid(*this).name() << endl;
        exit(3);
    }
};


class Identifer : public Expression {
private:
    string value;

public:
    Identifer(const char* v) : value(v) {
        label = "(identifier) " + value;
    }

    string& get_val() {return value;}
   
}


class Number : public Expression {
private:
    int value;

public:
    Number(int value) : value(value) {
        label = "(number) " + to_string(value);
    }

    string eval(Frame* st, Context* con) override {
        return to_string(value);
    }
};

class Boolean : public Expression {
private:
    bool value;

public:
    Boolean(bool v) : value(v) {
        label = "(boolean) ";
        label += (value ? "true" : "false");
    }
};

class Arthmetic : public Expression {
private:
    Oper op;
    Expression *left,
               *right;

public:
    Arthmetic(Expression* left, Oper op, Expression* right) :
    left(left), op(op), right(right) {
        label = "(arthmetic)";
    }

};

class Logical :  public Expression {
private:
    Oper op;
    Expression *left,
               *right;

public:
    Logical(Expression* left, Oper op, Expression* right) :
    left(left), op(op), right(right) {
        label = "(logical)";
    }
};

class Negation : public Expression {
private:
    Expression* right;

public:
    Negation(Expression* right) : right(right) {
        label = "(negation)";
        add(right);
    }
};


class Statment : public Node {
private:
    Statment* next = nullptr;

public:
    Statment() {
        label = "(statment)";
    }

    Statment* get_next() { return next;}
    void set_next(Statment* next_stmt) {
        if (next != nullptr) {
            errout << "err[ast]: unexpected error while setting next statment, (already set)" << endl;
            delete next;
            child.pop_back();
        }

        child.push_back(next_stmt);
        next = next_stmt;
    }

    virtual void exec(Frame* st, Context* con) {
        errout << "err[ast]: exec not implemented for " << typeid(*this).name() << endl;
        exit(5);
    }

};


class NullStatment : public Statment {
public:
    NullStatment() { label = "(nullstatment)"; }
    void exec(Frame* st, Context* con) override {}
};


class Block : public Statment {
private:
    Statment* body;
public:
    Block(Statment* body) : body(body) {
        label = "block";
        add(body);
    }
};


class Condition : public Statment {
private:
    Expression* cond;
    Block* ifblock;
    Block* ifblock;

public:
    Condition(Expression* cond, Block* ib, Block* eb) :
    cond(cond), ifblock(ib), elseblock(eb) {
        label = "(condition)";
        add(cond); add(ifblock), add(elseblock);
    }

};


class Loop : public Statment {
private:
    Expression* cond;
    Block* body;

public:
    Loop(Expression* cond, Block* body) :
    cond(cond), body(body) {
        label = "(loop)";
        add(cond);
        add(body);
    }
};

class Let : public Statment {
private:
    Identifier* id;
    Expression* expr;

public:
    Let(Identifier* id, Expression* expr) : id(id), expr(expr) {
        label = "(let)";
        add(id);
        add(expr);
    }
};

class Assignment : public Statment {
private:
    Identifier* id;
    Expression* expr;

public:
    Assignment(Identifier* id, Expression* expr) : id(id), expr(expr) {
        label = "(assignment)";
        add(id);
        add(expr);
    }
};

class Print : public Statment {
private:
    Expression* value;

public:
    Print(Expression* value) : value(value) {
        label = "(print)";
        add(value);
    }

    void exec(Frame* st, Context* con) override;
};

class ExpressionStatment : public Statment {
private:
    Expression* body;
public:
    ExpressionStatment(Expression* body) : body(body) {
        label = "(expr_statment)";
        add(body);
    }
};


#endif