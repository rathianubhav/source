#include <node.h>
#include <sstream>
#include <map>

Value
Identifier::eval(Context& cc)
{
    return cc.st.lookup(value);
}

Value
Number::eval(Context& cc)
{

    if (isInt(value)) return Value(atoi(value.c_str()));
    return Value(strtof(value.c_str(), 0));
}

Value
Bool::eval(Context& cc)
{
    return Value(value);
}

Value
String::eval(Context& cc)
{
    stringstream ss{""};
    for(int i = 0; i < value.size(); i++) {
        if (value.at(i) == '\\') {
            switch (value.at(i + 1)) {
                case 'n': ss << '\n'; i++; break;
                case 't': ss << "\t"; i++; break;
                case '"': ss << '\"'; i++; break;
                default:  ss << '\\';
            }
        } else {
            ss << value.at(i);
        }
    }
    return Value(ss.str());
}

Value
Array::eval(Context& cc)
{
    return Value(value);
}

Value
Access::eval(Context& cc)
{
    Value v = expr.eval(cc);
    Value arr = id.eval(cc);

    Value::check_type(v, INT_T);
    Value::check_type(arr, ARRAY_T);
    
    if (arr.Array()->size() <= v.Int()) {
        cout << "Error: out of index '" << id.get() << "' is of size " << arr.Array()->size() << " and trying to access " << v.Int() << " value"<< endl;
        exit(EXIT_FAILURE);
    }

    return Value(arr.Array()->at(v.Int())->eval(cc));
}

Value
Arithmetic::eval(Context& cc)
{
    auto l = left.eval(cc);
    auto r = right.eval(cc);

    switch(op) {
        case ADD : return l + r;
        case MUL : return l * r;
        case DIV : return l / r;
        case SUB : return l - r;
    }

    return Value();
}

Value
Logical::eval(Context& cc)
{
    auto l = left.eval(cc).Bool();
    auto r = right.eval(cc).Bool();

    switch(op) {
        case AND : return Value(l && r);
        case OR : return Value(l || r);
    }

    return Value(false);
}


Value
Compare::eval(Context& cc)
{
    auto l = left.eval(cc);
    auto r = right.eval(cc);

    switch(op) {
        case EQ : return Value(l == r);
        case NE : return Value(!(l == r));
        case LT : return Value(l < r);
        case GT : return Value(!(l < r));
        case LE : return Value((l == r)||(l < r));
        case GE : return Value((l == r)||!(l < r));
    }

    return Value(false);
}


Value
Negation::eval(Context& cc)
{
    return Value(!right.eval(cc).Bool());
}

Value
Method::eval(Context& cc)
{
    return Value(this, &cc.st);
}

Value
Call::eval(Context& cc)
{
    Value func_value = id.eval(cc);
    Value::check_type(func_value, FUNCTION_T);

    Closure func = func_value.Function();
    SymbolTable fcst(&cc.st);
    Context fc(fcst);

    fc.st.bind("ret", Value());

    auto reqargs = func.method_def->get_args();

    int fnc = reqargs->size(),
        clc = args.size();

    int min = fnc < clc ? fnc : clc;
    for(int i = 0; i < min; i++) {
        fc.st.bind(reqargs->at(i)->get(), args.at(i)->eval(cc));
    }

    if (fnc > min)
        for(int i = min; i < fnc; i++)
            fc.st.bind(reqargs->at(i)->get(), Value());
    else if (clc > min)
        for(int i = min; i < clc; i++)
            fc.st.bind("__extra_" + to_string((i - min) + 1) + "__", args.at(i)->eval(cc));

    fc.func++;
    func.method_def->get_body()->exec(fc);
    fc.func--;

    return fc.st.lookup("ret");
}

#include <unistd.h>

Value
Container::eval(Context &cc)
{
    st = new SymbolTable(&cc.st);
    Context ccc(st);

    for(auto a : data) {
        if (!st->isdefined(a->first->get()))
            st->bind(a->first->get(),a->second->eval(ccc));
        else
            st->rebind(a->first->get(), a->second->eval(ccc));
    }

    return Value(this, st);
}

Value
ContAccess::eval(Context& cc)
{
    Value ccon = cont.eval(cc);
    Value::check_type(ccon, CONTAINER_T);

    return ccon.Container().container_def->get_val(cid.get());
}