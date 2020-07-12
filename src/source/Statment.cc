#include <node.h>
#include <releax>

int
Block::exec(Context &cc)
{
    for(auto s : *body) {
        int exitvalue = s->exec(cc);
        switch(exitvalue) {
            case BREAK_STATMENT:
            case CONTINUE_STATMENT:
                if (cc.loop) {
                    return exitvalue;
                } else {
                    cout << "can not use break/countinue statment outside loop" << endl;
                }
                break;
            case RETURN_STATMENT:
                if (cc.func) {
                    return exitvalue;
                } else {
                    cout << "can not use return outside function" << endl;
                }
                break;
        }
    }

    return 0;
}

int
Let::exec(Context& cc)
{
    cc.st.bind(id.get(), expr.eval(cc));
    return 0;
}

int
Assign::exec(Context &cc)
{
    cc.st.rebind(id.get(), expr.eval(cc));
    return 0;
}

int
Print::exec(Context &cc)
{
    for(auto a : * expr) {
        a->eval(cc).repr(cout);
        cout << " ";
    }
    return 0;
}

int
ExprStatment::exec(Context& cc)
{
    body->eval(cc);
    return 0;
}

int
Condition::exec(Context &cc)
{
    if (expr.eval(cc).Bool()) {
        return ifblock.exec(cc);
    } else if (elseblock != nullptr) {
        return elseblock->exec(cc);
    }
    return 0;
}

int
Loop::exec(Context& cc)
{
    if (looptype == WHILE_LOOP) {
        cc.loop++;
        while (expr.eval(cc).Bool()) {
            int retval = body.exec(cc);
            if (retval == BREAK_STATMENT) {
                break;
            }
        }
        cc.loop--;

    } else if (looptype == FOR_LOOP) {
        cc.loop ++;
        pre->exec(cc);
        while(expr.eval(cc).Bool()) {
            int retval = body.exec(cc);
            if (retval == BREAK_STATMENT) {
                break;
            }
            incr->exec(cc);
        }
        cc.loop--;

    } else if (looptype == IN_LOOP) {
        Value exp = expr.eval(cc);
        if (!exp.oftype(ARRAY_T)) {
            cout << "Error: can't iterate of none reperable datatype '" << exp.get_type_str() << "'" << endl;
            exit(5);
        }
        cc.loop++;
        Value safe;
        if (!cc.st.isdefined(id->get())) {
            cc.st.bind(id->get(), Value());
        } else {
            safe = cc.st.lookup(id->get());
        }

        for(auto a : *exp.Array()) {
            cc.st.rebind(id->get(), a->eval(cc));
            int retval = body.exec(cc);
            if (retval == BREAK_STATMENT) {
                break;
            }
        }
        cc.loop--;
        
    } else {
        cout << "no yet implemented" << endl;
    }

    return 0;
}


extern "C" FILE *yyin;
extern int yyparse();
extern std::vector<Statment*> *tree;
int
Use::exec(Context& cc)
{
    FILE* oldptr = yyin;

    string mod = id.get();

    string modename = mod;

    char* c = getenv("SRC_MOD");
    if (c == NULL) {
        c = DEFAULT_MOD_PATH;
    }


    for (auto a : vector<string> {"./", "./mod/", c,}) {
        //cout << "checking " << a + "/" + mod + ".src" << endl;
        if (releax::is_exist(a + "/" + mod + ".src")) {
            mod = a + "/" + mod + ".src";
            break;
        }
    }

    if (!releax::is_exist(mod)) {
        cout << "Error: no module exist '" << mod << "'" << endl;
        exit(5);
    }

    yyin = fopen(mod.c_str(), "r");
    yyparse();

    SymbolTable modst = SymbolTable(&cc.st);
    auto modcc = Context(modst);

    for(auto c : *tree) {
        c->exec(modcc);
    }

    auto d = vector<ContainerData*>();
    for(auto s : modcc.st.bindings) {
        auto val = s.second;
        auto x = new ContainerData();
        x->first = new Identifier(s.first.c_str());
        switch (val.get_type()) {
            case INT_T:
            case FLOAT_T:
                x->second = new Number(to_string(val.Int()).c_str());   break;
            case STRING_T:
                x->second = new String(val.String().c_str());   break;
            case BOOL_T:
                x->second = new Bool(val.Bool()); break;
            case FUNCTION_T:
                x->second = val.Function().method_def; break;
            case CONTAINER_T:
                x->second = val.Container().container_def; break;

            default:
                cout << "invalid value in module" << endl;
                exit(EXIT_FAILURE);
        }

        d.push_back(x);
    }

    Value con = Container(d).eval(cc);
    cc.st.bind(modename, con);

    fclose(yyin);
    yyin = oldptr;

    return 0;
}

int
Debug::exec(Context &cc)
{
    if (id.get() == "print_st") {
        cc.st.print();
    } else {
        cout << "not yet implemented for '" << id.get() << "'" << endl;
    }
    return 0;
}