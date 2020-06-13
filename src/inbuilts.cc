#include <inbuilts.h>
#include <time.h>
#include <memory>

 /* InBuilts */
std::vector<InBuilt*> 
inbuilts = 
{
    new Typeof(), 
    new Sleep(),
    new System(),
    new Fopen(),
    new Fclose(),
    new Fgetc()
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
