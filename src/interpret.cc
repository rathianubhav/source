#include <interpreter.h>

extern bool error;
extern bool isdebug;

int yyparse();
std::vector<Statment*> *tree;

void 
source::interpreter::interprete(bool debug) {
    yyparse();

    int count = 0;

    auto cc = source::context::init(nullptr);
    for (auto s : *tree) {
        if (debug) {
            std::cout << "["<< count++ << "] statment " << std::endl;
            sleep(0.5);
        }
        s->exec(cc);
    }
}


Value
Identifier::eval(context::Context* cc)
{
    return cc->st->lookup(value);
}

Value
Number::eval(context::Context* cc)
{
    int dotind = value.rfind('.');
    if (dotind == std::string::npos) return Value(atoi(value.c_str()));
    return Value(strtod(value.c_str(), 0));
}

Value
Bool::eval(context::Context* cc)
{
    return Value(value);
}

Value
String::eval(context::Context* cc)
{
    return Value((char*)value.c_str());
}

Value
Arithmetic::eval(context::Context* cc)
{
    auto l = left->eval(cc);
    auto r = right->eval(cc);

    switch(op) {
        case ADD: return l+r;
        case SUB: return l-r;
        case MUL: return l*r;
        case DIV: return l/r;
        default:
            error = true;
            err << "err[internal] illegal arithmetic operator" << std::endl;
    }
    return Value();
}

Value
Compare::eval(context::Context* cc)
{
    auto l = left->eval(cc);
    auto r = right->eval(cc);

    switch (op) {
        case EQ: return Value(l == r);
        case NE: return Value(!(l == r));
        case LT: return Value(l < r);
        case GT: return Value(!(l < r));
        case LE: return Value((l == r) || (l < r));
        case GE: return Value((l == r) || !(l < r));
    }
}



Value
Method::eval(context::Context* cc)
{
    return Value(this, cc->st);
}

Value
Call::eval(context::Context* cc)
{
    Closure func = cc->st->lookup(id->get()).Func();
    source::ST* funenv = new source::ST(func.env);

    funenv->bind("ret", Value());

    auto func_req_args = func.func->get_args();
    int fnc = func_req_args->size(),
        clc = arg->size();

    int min = fnc < clc ? fnc : clc;
    for(int i = 0; i < min; i++) {
        funenv->bind(func_req_args->at(i)->get(), arg->at(i)->eval(cc));
    }

    if (fnc > min) {
        for(int i = min; i < fnc; i++) {
            funenv->bind(func_req_args->at(i)->get(), Value());
        }
    } else if (clc > min) {
        for(int i = min; i < clc; i++) {
            funenv->bind("__extra_" + std::to_string((i - min) + 1) + "__", arg->at(i)->eval(cc));
        }
    }

    auto fc = source::context::init(funenv);
    func.func->get_body()->exec(fc);
    return funenv->lookup("ret");
}

void
Block::exec(context::Context* cc)
{
    for(auto a : *body) {
        a->exec(cc);
    }
}

void
Condition::exec(context::Context* cc)
{
    if (expr->eval(cc).Bool()) {
        ifblock->exec(cc);
    } else if (elseBlock != nullptr) {
        elseBlock->exec(cc);
    }
}

Value
Typeof::eval(context::Context* cc)
{
    switch (expr->eval(cc).getType()) 
    {
        case INT_T: return Value("int");
        case FLOAT_T: return Value("float");
        case FUNC_T: return Value("func");
        case STR_T: return Value("str");
        case BOOL_T: return Value("bool");
        case ANY_T: return Value("any");
        case NONE_T: return Value("none");
    }
}
void
Loop::exec(context::Context* cc)
{
    while(expr->eval(cc).Bool()) {
        body->exec(cc);
    }
}


void
Let::exec(context::Context* cc)
{
    cc->st->bind(id->get(), expr->eval(cc));
}


void
Assign::exec(context::Context* cc)
{
    cc->st->rebind(id->get(), expr->eval(cc));
}

void
Print::exec(context::Context* cc)
{
    expr->eval(cc).repr(std::cout);
    std::cout << std::endl;
}

void
ExpressionStatment::exec(context::Context* cc)
{
    body->eval(cc);
}

