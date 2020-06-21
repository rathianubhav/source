#include <inbuilts.h>
#include <time.h>
#include <memory>
#include <dlfcn.h>

 /* InBuilts */
std::vector<InBuilt*> 
inbuilts = 
{
    new Typeof(), 
    new Sleep(),
    new System(),
    new Fopen(),
    new Fclose(),
    new Fputc(),
    new Fgetc(),
    new Range(),
    new Append(),
    new CLib(),
};

Value
Typeof::run(std::vector<Value> args)
{
    switch (args.at(0).getType()) {
        case INT_T: return Value("int");
        case FLOAT_T: return Value("float");
        case STR_T: return Value("string");
        case BOOL_T: return Value("bool");
        case NONE_T: return Value("null");
        case ANY_T: return Value("any");
        case FUNC_T: return Value("func");
        case ARRAY_T: return Value("array");
        case DICT_T: return Value("container");
    }

    return Value("null");
}


Value
Sleep::run(std::vector<Value> args)
{
    auto t = args.at(0);
    switch(t.getType()) {
        case INT_T: return Value((int)sleep(t.Int()));
        case FLOAT_T: return Value((double)sleep(t.Float()));
        default:
            err << "need int or float" << std::endl;
            

    }
    return Value();
}

Value
System::run(std::vector<Value> args)
{
    std::array<char, 128> buffer;
    std::string result;

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(args.at(0).Str().c_str(), "r"), pclose);
    if (!pipe) {
        return Value("popen() failed");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return Value((char*)result.c_str());
}

Value
Fopen::run(std::vector<Value> args)
{
    FILE* fptr;
    fptr = fopen(args.at(0).Str().c_str(), args.at(1).Str().c_str());
    if (fptr) {
        return Value(fptr);
    }
    return Value(false);
}


Value
Fclose::run(std::vector<Value> args)
{
    FILE* fptr;
    if (args.at(0).getType() != ANY_T) {
        return Value(false);
    }
    fptr = (FILE*)args.at(0).Any();

    if (fptr) {
        fclose(fptr);
        return Value(true);
    }

    return Value(false);
}

Value
Fgetc::run(std::vector<Value> args)
{
    FILE* fptr;

    if (args.at(0).getType() != ANY_T) {
        return Value(false);
    }
    fptr = (FILE*)args.at(0).Any();

    if (fptr) {
        auto c = getc(fptr);
        if (c == -1) {
            return Value();
        }
        return Value(std::string(1, c));
    }

    return Value(false);
}

Value
Fputc::run(std::vector<Value> args)
{
    FILE* fptr;
    if (args.at(0).getType() != ANY_T) {
        return Value(false);
    }

    fptr = (FILE*)args.at(0).Any();
    if (fptr) {
        fputc(args.at(1).Str()[0],fptr);
        return Value(true);
    }

    return Value(false);
}

Value
Range::run(std::vector<Value> args)
{
    auto arr = new std::vector<Expression*>();
    switch (args.at(0).getType()) {
        case INT_T:
            for(int i = 0; i < args.at(0).Int(); i++) {
                arr->push_back(new Number(i));
            }

            break;

        case FLOAT_T:
            for(double i = 0; i < args.at(0).Float(); i++) {
                arr->push_back(new Number(i));
            }

            break;

        case STR_T:
            for(auto i : args.at(0).Str()) {
                arr->push_back(new String(std::string(1, i).c_str()));
            }
            break;
    }

    return Value(arr);
}


Value
Append::run(std::vector<Value> args)
{
    if (args.at(0).getType() != ARRAY_T) {
        err << "Can't append in none Arrary value" << std::endl;
        return Value();
    }
    switch(args.at(1).getType()) {
        case INT_T: args.at(0).Arr()->push_back(new Number(args.at(1).Int())); break;
        case FLOAT_T: args.at(0).Arr()->push_back(new Number(args.at(1).Float())); break;
        case BOOL_T: args.at(0).Arr()->push_back(new Bool(args.at(1).Bool())); break;
        case STR_T: args.at(0).Arr()->push_back(new String(args.at(1).Str().c_str())); break;
    }

    return Value(args.at(0));
}


Value
CLib::run(std::vector<Value> args)
{

    void* handler = dlopen(args.at(0).Str().c_str(), RTLD_LAZY);
    if (!handler) {
        std::cout << "can't load library: " << dlerror() << std::endl;
        return Value(false);
    }

    create_inbuilt* built_func = (create_inbuilt*) dlsym(handler, (args.at(1).Str() + "_create").c_str());
    destroy_inbuilt* destroy_func = (destroy_inbuilt*) dlsym(handler, (args.at(1).Str() + "_destroy").c_str());

    if (! built_func || !destroy_func) {
        std::cout << "can't load symbol: " << dlerror() << std::endl;
        return Value(false);
    }

    InBuilt* func = built_func();
    auto retval = func->run(args);

    destroy_func(func);

    return retval;
}