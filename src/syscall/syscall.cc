#include <unistd.h>
#include <sys/syscall.h>

#include <value.h>
#include <vector>
#include <string>
#include <inbuilts.h>

extern "C"
Value
Syscall(vector<Value> args)
{
    Value scval = args.at(0);
    Value::check_type(scval, STRING_T);

    string scstr = scval.String();
    if (scstr == "write") {
        if (args.size() < 2) {
            cout << "required 3 argumented but provided " << args.size() << endl;
            exit(EXIT_FAILURE);
        }
        Value fd = args.at(1);
        Value buf = args.at(2);

        Value::check_type(fd, INT_T);
        Value::check_type(buf, STRING_T);

        syscall(SYS_write, fd.Int(), buf.String().c_str(), buf.String().size());
        return Value();
    } else {
        cout << "illegal syscall " << scstr << endl;
    }
    return Value();
}