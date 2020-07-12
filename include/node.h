#ifndef __NODE_H__
#define __NODE_H__

#include "context.h"
#include <vector>

#define DEFAULT_MOD_PATH "/usr/lib/source/modules/"

enum Operator {
    ADD, SUB, MUL, DIV,
    LT, GT, LE, GE,
    AND, OR, NOT, EQ, NE
};

#define BREAK_STATMENT 11
#define CONTINUE_STATMENT 12
#define RETURN_STATMENT 13

class Node {
private:
protected:
    
public:
    string label;

    Node() : label("node") {}
    virtual ~Node() {}
};

class Expression : public Node {
public:
    virtual Value eval(Context &cc) {
        cout << "Error: eval not yet implemented in " << typeid(this).name() << endl;
        cout << label << endl;
        return Value();
    }
};

class Identifier : public Expression {
private:
    string value;
public:
    Identifier(const char* val) : value(val) {
        label = "identifier(" + value + ")";
    }
    string& get() {return value;}
    virtual Value eval(Context& cc) override;
};

class Number : public Expression {
private:
    string value;
public:
    Number(const char* val) : value(val) {
        label = "number(" + value + ")";
    }
    virtual Value eval(Context &cc) override;
};

class Bool : public Expression {
private:
    bool value;
public:
    Bool(bool val) : value(val) {
        label = "bool(";
        label += (value ? "true" : "false");
        label += ")";
    }
    virtual Value eval(Context &cc) override;
};

class String : public Expression {
private:
    string value;
public:
    String(const char* s) : value(s) {
        label = "string(" + value + ")";
    }
    virtual Value eval(Context &cc) override;
};

class Array : public Expression {
private:
    vector<Expression*> *value;
public:
    Array(vector<Expression*> *val) : value(val)
    {
        label = "array";
    }
    virtual Value eval(Context &cc) override;
};

class Access : public Expression {
private:
    Expression& expr;
    Identifier& id;
public:
    Access(Identifier& id, Expression& val) : id(id), expr(val)
    {
        label = "acess(" + id.get() + ")";
    }
    virtual Value eval(Context &cc) override;
};

class Null : public Expression {
private:
public:
    Null() {
        label = "null";
    }
    virtual Value eval(Context &cc) override {
        return Value();
    }

};

class Arithmetic : public Expression {
private:
    Operator op;
    Expression &left, &right;
public:
    Arithmetic(Expression &left, Operator op, Expression &right)
     : left(left), right(right), op(op) {
         label = "expression";
     }

     virtual Value eval(Context &cc) override;
};

class Logical : public Expression {
private:
    Operator op;
    Expression &left, &right;
public:
    Logical(Expression& l, Operator op, Expression& r)
    : left(l), op(op), right(r) {
        label = "logical";
    }

    virtual Value eval(Context &cc) override;
};

class Compare : public Expression {
private:
    Operator op;
    Expression &left, &right;
public:
    Compare(Expression& l, Operator op, Expression& r)
    : left(l), op(op), right(r) {
        label = "compare";
    }

    virtual Value eval(Context &cc) override;
};

class Negation : public Expression {
private:
    Expression &right;

public:
    Negation(Expression& r)
    : right(r) {
        label = "negation";
    }

    virtual Value eval(Context &cc) override;
};

class Statment : public Node {
private:
public:
    Statment() {
        label = "statment";
    }

    virtual int exec(Context& cc) {
        cout << "Error: exec() not yet implemented for " << typeid(*this).name() << endl;
        return -1;
    }
};

class Block : public Statment {
private:
    vector<Statment*> *body;
public:
    Block(vector<Statment*> *body) : body(body) {
        label = "block";
    }

    vector<Statment*> *get_body() {return body;}
    virtual int exec(Context& cc) override;

    virtual ~Block() {
        for(auto a : *body) delete a;
        body->clear();
    }
};

class Condition : public Statment {
private:
    Expression &expr;
    Block &ifblock, *elseblock;
public:
    Condition(Expression &expr, Block& ib, Block* eb = nullptr)
    : expr(expr), ifblock(ib), elseblock(eb) {
        label = "condition";
    }

    virtual int exec(Context& cc) override;
};

enum LOOP_T {
    FOR_LOOP, WHILE_LOOP, IN_LOOP
};

class Loop: public Statment {
private:
    Expression& expr;
    Statment *pre, *incr;
    Block& body;
    Identifier* id;
    LOOP_T looptype;

public:
    Loop(Expression& expr, Block& body)
     : expr(expr), body(body), looptype(WHILE_LOOP)
     { label = "while_loop";}

    Loop(Statment& pre, Expression& expr, Statment& incr, Block& body)
    : pre(&pre), expr(expr), incr(&incr), looptype(FOR_LOOP), body(body)
    { label = "for_loop";}

    Loop(Identifier& id, Expression& expr, Block& body)
    : id(&id), expr(expr), body(body), looptype(IN_LOOP)
    { label = "in_loop"; }

    virtual int exec(Context& cc) override;

};


class Break : public Statment {
private:

public:
    Break() { label = "break";}
    virtual int exec(Context& cc) override {
        return BREAK_STATMENT;
    }
};

class Continue : public Statment {
private:

public:
    Continue() { label = "continue";}
    virtual int exec(Context& cc) override {
        return CONTINUE_STATMENT;
    }
};


class Return : public Statment {
private:
    Expression& expr;
public:
    Return(Expression& expr) 
    : expr(expr)
    { label = "return";}
    virtual int exec(Context& cc) override {
        cc.st.rebind("ret", expr.eval(cc));
        return RETURN_STATMENT;
    }
};

class Let : public Statment {
private:
    Identifier &id;
    Expression &expr;
public:
    Let(Identifier& id, Expression& expr)
    :   id(id), expr(expr) {
        label = "let(" + id.get() + ")";
    }
    virtual int exec(Context& cc) override;
};

class Assign : public Statment {
private:
    Identifier &id;
    Expression &expr;
public:
    Assign(Identifier& id, Expression& expr)
     : id(id), expr(expr) {
         label = "assign(" + id.get() + ")";
     }

    virtual int exec(Context& cc) override;
};

class Print : public Statment {
private:
    vector<Expression*> *expr;
public:
    Print(vector<Expression*> *expr) :
        expr(expr) {
            label = "print";
        }
    virtual int exec(Context& cc) override;

    virtual ~Print() {
        for(auto a : *expr) delete a;
        expr->clear();
    }
};

class ExprStatment : public Statment {
private:
    Expression* body;
public:
    ExprStatment(Expression* body) :
        body(body)
    {
        label = "expr_statment";
    }

    virtual int exec(Context& cc) override;
};


class Method : public MethodDef, public Expression {
private:
    vector<Identifier*> *id;
    Statment* body;
public:
    SymbolTable* envst;
    Method() {}
    Method(vector<Identifier*> *id, Statment *body)
     : id(id), body(body) {}
    
    vector<Identifier*> *get_args() {return id;}
    Statment* get_body() {return body;}

    virtual Value eval(Context& cc) {
        return Value(this, &cc.st);
    }
};

class Call : public Expression {
private:
    vector<Expression*> &args;
    Identifier& id;
public:
    Call(Identifier& id, vector<Expression*> &a)
     : id(id), args(a) {
         label = "call(" + id.get() + ")";
     }
    
    virtual Value eval(Context &cc) override;

};

class Use : public Statment {
private:
    Identifier& id;
public:
    Use(Identifier& id) : id(id) {
        label = "use(" + id.get() + ")";
    }

    virtual int exec(Context &cc) override;
};


class Debug : public Statment {
private:
    Identifier& id;
public:
    Debug(Identifier& id) : id(id) {
        label = "debug(" + id.get() + ")";
    }
    virtual int exec(Context &cc) override;
};

class Container : public ContainerDef, public Expression {
private:
    Identifier& id;
    SymbolTable* st;
public:
    Container(Identifier& id, vector<ContainerData*> &d)
    : id(id), data(d) {
        label = "container(" + id.get() + ")";
    }

    vector<ContainerData*> &data;

    Value get_val(const string& i) {
        return st->lookup(i);
    }
    
    virtual Value eval(Context& cc) override;
};

class ContAccess : public Expression {
private:
    Expression& cont;
    Identifier &cid;
public:
    ContAccess(Expression& c, Identifier& i) : cont(c), cid(i) {
        label = "ContainerAccess";
    }

    virtual Value eval(Context& cc) override;
};

class Cmod : public Expression {
private:
    vector<Expression*> &exprs;
public:
    Cmod(vector<Expression*> *exprs) : exprs(*exprs) {
        label = "cmod()";
    }

    virtual Value eval(Context& cc) override;
};


#endif