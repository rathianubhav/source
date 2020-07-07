#include <inbuilts.h>
#include <unistd.h>
#include <node.h>


vector<InBuilt*> inbuilts = vector<InBuilt*>
{
    new Sleep(),
    new Append(),
    new Len(),
    new IsType(INT_T), new IsType(FLOAT_T), new IsType(STRING_T), new IsType(BOOL_T), new IsType(FUNCTION_T), new IsType(ARRAY_T), new IsType(ANY_T),
};

Value
Sleep::run(vector<Value> args)
{   
    Value::check_type(args.at(0), INT_T);
    return Value((int)sleep(args.at(0).Int()));
}

Value
Append::run(vector<Value> args)
{
    Value iterable = args.at(0);
    Value val = args.at(1);
    Expression* expr;

    if (!iterable.oftype(ARRAY_T) &&
        !iterable.oftype(STRING_T)) {
        cout << "Error: appending in none seperable datatype '" << iterable.get_type_str() << "' is not allowed" << endl;
        exit(EXIT_FAILURE);
    }

    switch (val.get_type()) {
        case INT_T: expr = new Number(to_string(val.Int()).c_str()); break;
        case FLOAT_T: expr = new Number(to_string(val.Float()).c_str()); break;
        case STRING_T: expr = new String(val.String().c_str()); break;
        case BOOL_T: expr = new Bool(val.Bool()); break;
        case ARRAY_T: expr = new Array(val.Array()); break;
        default:
            cout << "Error: illegal value of datatype '"  << val.get_type_str() << "' can't be append in " << iterable.get_type_str() << endl;
            exit(EXIT_FAILURE);
    }

    iterable.Array()->push_back(expr);

    return iterable;
}

Value
Len::run(Args args)
{
    switch (args.at(0).get_type()) {
        case INT_T: return Value((int)sizeof(int));
        case FLOAT_T: return Value((int) sizeof(float));
        case STRING_T: return Value((int) args.at(0).String().length());
        case ARRAY_T: return Value((int) args.at(0).Array()->size());
        case BOOL_T: return Value((int) sizeof(bool));
    }
    
    cout << "Error: len() for '" << args.at(0).get_type_str() << "' not yet implemented" << endl;
    exit(EXIT_FAILURE);

    return Value(0);
}

Value
IsType::run(Args args)
{
    return Value(t == args.at(0).get_type());
}