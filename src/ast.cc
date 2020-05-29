#include "../include/ast"
#include <string.h>

#ifdef __releax__
#include <releax>
#endif
int yyparse();

void
AST::add(AST* c) {
    child.push_back(c);
}

void
AST::add_to_dot(ostream& fptr, int& next_node)
{
    int root = next_node;
    ++next_node;
    fptr << "\tn" << root << " [shape=rectangle label=\"" << node_label << "\"];" << endl;
    for (auto a : child) {
        int c = next_node;
        a->add_to_dot(fptr, next_node);
        fptr << "\tn" << root << " -> n" << c << ";" << endl;
    }
}

void 
AST::write_dot(const string& file_name)
{
    ofstream fptr(file_name);
    int nodes = 1;
    fptr << "digraph AST {" << endl;
    add_to_dot(fptr, nodes);
    fptr << "}" << endl;
    fptr.close();
}


Value
Binary::eval(Frame* st) {    
    auto lv = left->eval(st);
    auto rv = right->eval(st);

    if (lv.get_type() == rv.get_type() == INT_T) {
        auto l = lv.intval();
        auto r = rv.intval();

        switch (op) {
            case ADD: return Value(l + r);
            case SUB: return Value(l - r);
            case MUL: return Value(l * r);
            case DIV:
                if (r != 0) return Value(l / r);
                else if (!error) {
                    error = true;
                    cout << "arthmetical err: divide by zero" << endl;
                }
                break;

            default:
                cout << "internal err: illegal arithmetic operator" << endl;
        }
    } else 
    
    if (lv.get_type() == rv.get_type() == FLOAT_T) {
        auto l = lv.floatval();
        auto r = rv.floatval();

        switch (op) {
            case ADD: return Value(l + r);
            case SUB: return Value(l - r);
            case MUL: return Value(l * r);
            case DIV:
                if (r != 0) return Value(l / r);
                else if (!error) {
                    error = true;
                    cout << "arthmetical err: divide by zero" << endl;
                }
                break;

            default:
                if (!error) {
                    cout << "internal err: illegal arithmetic operator" << endl;
                    error = true;
                }
        }

    } else {
        if (!error) {
            cout << "invalid compariason b/w " << endl;
            lv.repr(cout);
            rv.repr(cout);

            error = true;
        }
        
    }
    return Value();
}

Value
Compare::eval(Frame *st) {
    
    auto lv = left->eval(st);
    auto rv = right->eval(st);

    if (lv.get_type() == rv.get_type() == INT_T) {
        auto l = lv.intval();
        auto r = rv.intval();
        switch (op) {
            case EQ: return Value(l == r);
            case NE: return Value(l != r);
            case GT: return Value(l > r);
            case GE: return Value(l >= r);
            case LE: return Value(l <= r);
            case LT: return Value(l < r);
            default:
                cout << "internal err: illegal logical operator" << endl;
        }
    } else 
    
    if (lv.get_type() == rv.get_type() == FLOAT_T) {
        auto l = lv.floatval();
        auto r = rv.floatval();

        switch (op) {
            case EQ: return Value(l == r);
            case NE: return Value(l != r);
            case GT: return Value(l > r);
            case GE: return Value(l >= r);
            case LE: return Value(l <= r);
            case LT: return Value(l < r);
            default:
                cout << "internal err: illegal logical operator" << endl;
        }

    } else
    
    
    if (lv.get_type() == rv.get_type() == INT_T) {
        auto l = lv.str();
        auto r = rv.str();

        switch (op) {
            case EQ: return Value((bool)!strcmp(l, r));
            case NE: return Value((bool)strcmp(l, r));
            case GT: return Value(strlen(l) > strlen(r));
            case GE: return Value(strlen(l) >= strlen(r));
            case LE: return Value(strlen(l) <= strlen(r));
            case LT: return Value(strlen(l) < strlen(r));
            default:
                cout << "internal err: illegal logical operator" << endl;
        }
    } else {
        cout << "invalid compariason b/w " << endl;
        lv.repr(cout);
        rv.repr(cout);
    }
    return Value(false);
}


Value
Boolean::eval(Frame *st) {
    bool first = left->eval(st).tf();
    if ((first && op == OR) || (!first && op == AND)) {
        return Value(first);
    } else {
        return right->eval(st);
    }
}

Value 
Read::eval(Frame *sr) {
    if (showPrompt) {
        cout << "input > ";
    }

    string inp;
    cin >> inp;
    bool d = false;
    for(auto c: inp) {
        if (!isdigit(c)) {
            if (c == '.') {
                if (!d) {
                    d = true;
                }
            } else {
                return Value((char*)inp.c_str());
            }
            
        }
    }

    if (d) {
        return Value((double)atof(inp.c_str()));
    }
    return Value((long)atoi(inp.c_str()));
}

void
Stmt::set_next(Stmt* next_stmt)
{
    if (next != nullptr) {
        cout << "parser err: next statment already set" << endl;
        delete next;
        child.pop_back();
    }

    child.push_back(next_stmt);
    next = next_stmt;
}

void
UseStmt::exec(Frame *st)
{
    #ifdef __releax__
        extern FILE* yyin;
        extern Stmt* tree;
        extern Frame global;
        extern vector<Stmt*> program;
        //auto state = tree;

        auto original = yyin;

        //cout << "tree node: " << tree->node_label << endl;

        string mod_path = "data/" + module_name + ".src";
        if (!releax::is_exist(mod_path)) {
            if (!error) {
                cout << "unable to load module " << module_name << endl;
                error = true;
            }
        } else {
            //cout << "opening file " << module_name << endl;
            yyin = fopen(mod_path.c_str(), "r");
            while (true)
            {
                tree = nullptr;
                error = false;

                if (yyparse() != 0) error = true;
                if (!error && tree != nullptr) {
                    //cout << "executing Node " << tree->node_label << endl;
                    //cout << tree->node_label << endl;
                    tree->exec(&global);
                    program.push_back(tree);
                    
                } else {
                    break;
                }
                //cout << "in loop" << endl;
            }
            fclose(yyin);
        }
        //cout << "tree state" << tree->node_label << endl;
        yyin = original;
    #else
        cout << "Not workable in non-unix os" << endl;
    #endif
    get_next()->exec(st);
}

void
Block::exec(Frame *st)
{
    body->exec(st);
    get_next()->exec(st);
}

void
IfStmt::exec(Frame* st)
{
    if (clause->eval(st).tf()) ifblock->exec(st);
    else elseblock->exec(st);
    get_next()->exec(st);
}

void
ForStmt::exec(Frame* st)
{
    switch(type) {
        case COND_T:    while(clause->eval(st).tf()) body->exec(st); break;
        case RANGE_T:
            
            if (!st->exist(id->get())) {
                st->bind(id->get(), Value((long)0));
            }
            for(int i = st->lookup(id->get()).intval(); i < clause->eval(st).intval(); i++) {
                st->rebind(id->get(),Value((double)i));
                body->exec(st);
            }

            break;
    }
    
    get_next()->exec(st);
}

void
LetStmt::exec(Frame* st)
{
    st->bind(lhs->get(), rhs->eval(st));
    get_next()->exec(st);
}

void
Assign::exec(Frame *st)
{
    st->rebind(lhs->get(), rhs->eval(st));
    get_next()->exec(st);
}

void
Print::exec(Frame* st)
{
    for (auto a : val) {
        Value res = a->eval(st);
        if (!error) {
            res.repr(cout);
        }
    }
    
    get_next()->exec(st);
}

void
PrintLn::exec(Frame* st)
{
    for (auto a : val) {
        Value res = a->eval(st);
        if (!error) {
            res.repr(cout);
        }
    }
    cout << endl;
    get_next()->exec(st);
}

void
ExpStmt::exec(Frame* st)
{
    body->eval(st);
    get_next()->exec(st);
}


Value
FunCall::eval(Frame* st)
{

    Closure func = fun->eval(st).func();
    Frame* funenv = new Frame(func.env);
    funenv->bind("ret", Value());
    auto parm_req = func.func->get_args();
    if (args.size() != parm_req.size()) {
        if (!error) {
            cout << "err: required '" << parm_req.size() << "' but '" << args.size() << "' given" << endl;
            error = true;
        }
        return Value();
    } else {
        for (int i = 0; i < args.size(); i++) {
            funenv->bind(parm_req.at(i)->get(), args.at(i)->eval(st));
        }
    }
    func.func->get_body()->exec(funenv);
    return funenv->lookup("ret");

}