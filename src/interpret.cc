#include <interpreter.h>
#include <inbuilts.h>
extern bool error;
extern bool isdebug;

int yyparse();
std::vector<Statment*> *tree;
extern std::vector<InBuilt*> inbuilts;
void 
source::interpreter::interprete(std::vector<std::string> args, bool debug) {
    yyparse();

    int count = 0;

    auto cc = source::context::init(nullptr);

   
    for(auto a : inbuilts) {
        cc->st->bind(a->getID(), Value(a, nullptr));
    }

    for (auto s : *tree) {
        if (debug) {
            std::cout << "["<< count++ << "] statment " << std::endl;
            sleep(0.5);
        }
        s->exec(cc);
    }

    if (!cc->st->defined("main")) {
        err << "no main function defined" << std::endl;
        return;
    }

    auto v = new std::vector<Expression*>;
    for(auto a : args) {
        v->push_back(new String(a.c_str()));
    }

    auto main_func = cc->st->lookup("main").Func();
    auto ar = main_func.func->get_args();
    main_func.env->bind(ar->at(0)->get(), Value(v));
    main_func.func->get_body()->exec(cc);
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
Array::eval(context::Context* cc)
{
    if (id) {
        return Value(cc->st->lookup(id->get()));
    }
    return Value(exprs);
}

Value
Access::eval(context::Context* cc)
{
    auto arr = cc->st->lookup(id->get());
    if (arr.getType() != ARRAY_T) {
        err << "err[key]: not a iterable object" << std::endl;
        return Value();
    }
    if (expr->eval(cc).Int() > arr.Arr()->size()) {
        err << "err[key]: out-of-index" << std::endl;
        return Value();
    }
    return arr.Arr()->at(expr->eval(cc).Int())->eval(cc);
}

Value
Container::eval(context::Context* cc)
{
    return Value(value);
}

Value
ContainerEval::eval(context::Context* cc)
{
    auto d = cont->eval(cc).Dict();
    for(auto i = d->begin(); i != d->end(); i++) {
        auto a =*i;
        if (a->first->get() == var->get()) {
            return a->second->eval(cc);
        }
    }

    return Value();
}

void
Array::respr(context::Context* cc, std::ostream& out)
{
    for (auto a : *exprs) {
        a->eval(cc).repr(out);
    }
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
Logical::eval(context::Context* cc)
{
    auto l = left->eval(cc).Bool();
    auto r = right->eval(cc).Bool();

    switch(op) {
        case AND:   return Value(l && r);
        case OR:   return Value(l || r);
    }
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
Negation::eval(context::Context* cc)
{
    return Value(! expr->eval(cc).Bool());
}


Value
Method::eval(context::Context* cc)
{
    return Value(this, cc->st);
}

Value
Call::eval(context::Context* cc)
{
    Closure func = id->eval(cc).Func();
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


void
Loop::exec(context::Context* cc)
{
    switch(type) {
        case UNTIL: 
            while(expr->eval(cc).Bool()) {
                body->exec(cc);
            } 
            break;
        case INLOOP: {
            Value backup;
            if (cc->st->defined(id->get())) {
                backup = cc->st->lookup(id->get());
            } else {
                cc->st->bind(id->get(), Value());
            }

            int x = 0;
            for(auto a : *arr->eval(cc).Arr()) {
                cc->st->rebind(id->get(), Value(a->eval(cc)));
                body->exec(cc);
            }
            break;
        }

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
    for(auto a : *expr) {
        a->eval(cc).repr(std::cout);
    }
}

void
Println::exec(context::Context* cc)
{
   for(auto a : *expr) {
        a->eval(cc).repr(std::cout);
    }
    std::cout << std::endl;
}

void
ExpressionStatment::exec(context::Context* cc)
{
    body->eval(cc);
}

extern "C" FILE *yyin;

void
Use::exec(context::Context* cc)
{
    auto oldptr = yyin;

    std::string mod = modname->eval(cc).Str();
    std::string mod_name = mod;
    mod += ".src";

    char* c = std::getenv("SRC_MOD");
    if (c == NULL) {
        c = "/usr/lib/modules/vendor";
    }
        

    for (auto a : std::vector<std::string>{ MOD_LOC, "./", c }) {
        if (releax::is_exist(a + "/" + mod)) {
            mod = a + "/" + mod;
            break;
        }
    }

    if (!releax::is_exist(mod)) {
        err << mod << " not exist" << std::endl;
        return;
    }

    yyin = fopen(mod.c_str(), "r");
    yyparse();

    auto modcc = source::context::init(cc->st);
    for(auto c : *tree) {
        c->exec(modcc);
    }

    auto d = new std::vector<dict*>();
    for(auto s : modcc->st->bindings) {
        auto val = s.second;
        auto x = new dict();
        x->first = new Identifier(s.first.c_str());
        switch (val.getType()) {
            case INT_T:
            case FLOAT_T: x->second = new Number(std::to_string(val.Int()).c_str()); break;
            case STR_T: x->second = new String(val.Str().c_str()); break;
            case BOOL_T: x->second = new Bool(val.Bool()); break;
            case FUNC_T: x->second = val.Func().func; break;
            case DICT_T: x->second = new Container(val.Dict()); break;
        }

        d->push_back(x);
    }

    cc->st->bind(mod_name, Value(d));
    //fclose(yyin);
    delete modcc;

    yyin = oldptr;
}