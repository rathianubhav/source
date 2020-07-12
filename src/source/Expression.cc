#include <node.h>
#include <sstream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <dlfcn.h>

Value
Identifier::eval(Context& cc)
{
    int rindex = value.rfind('.');
    if (rindex != string::npos) {
        // '.' -> container call
        stringstream ss(value);
        string curcont;
        vector<string> contlist;

        while(getline(ss, curcont, '.')) {
            cout << "pushing " << curcont << endl;
            contlist.push_back(curcont);
        }

        cout << "Containers Count: " << contlist.size() << endl;

        SymbolTable* curst = &cc.st;
        string last = contlist.at(contlist.size() - 1);
        cout << "Last Element: " << last << endl;
        contlist.erase(contlist.end());
        for(auto c : contlist) {
            cout << "checking container " << c << endl;
            if(!curst->isdefined(c)) {
                cout << "container " << c << " is not defined" << endl;
            }

            Value contval = curst->lookup(c);
            Value::check_type(contval, CONTAINER_T);
            curst = contval.Container().environment;
        }

        cout << "is defined: " <<curst->isdefined(last) << endl;
        curst->print();
        return curst->lookup(last);
    } else
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


    cout << "I am working here" << endl;

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
    Value container = cont.eval(cc);
    Value::check_type(container, CONTAINER_T);
    return container.Container().container_def->get_val(cid.get());
}

Value
Cmod::eval(Context& cc)
{
    if (exprs.size() <= 1) {
        cout << "Error: cmod() require atleast 2 arguments but provided " << exprs.size() << endl;
        exit(EXIT_FAILURE);
    }

    Value (*cfunc_ptr)(vector<Value>&);

    Value clib = exprs.at(0)->eval(cc);
    Value::check_type(clib, STRING_T);
    Value cfunc = exprs.at(1)->eval(cc);
    Value::check_type(cfunc, STRING_T);

    void* handler = dlopen(clib.String().c_str(), 0x00001);
    if (handler == NULL) {
        cout << "Error: dlopen() unable to load specified library '" << clib.String() << "' - " << dlerror() << endl;
        exit(EXIT_FAILURE);
    }

    cfunc_ptr = (Value(*)(vector<Value>&)) dlsym(handler, cfunc.String().c_str());
    if (cfunc_ptr == NULL) {
        cout << "Error: dlsym() unable to load '" << cfunc.String() << "' symbol - " << dlerror() << endl; 
        exit(EXIT_FAILURE);
    }

    vector<Value> args;
    for(auto i = exprs.begin() + 2; i != exprs.end(); i++) {
        auto e = *i;
        args.push_back(e->eval(cc));
    }

    Value retval = cfunc_ptr(args);
    dlclose(handler);

    return retval;
}