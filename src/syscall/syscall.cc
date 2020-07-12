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
    cout << args.size() << endl;
    
}