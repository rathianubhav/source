#include <value.h>
#include <unistd.h>
#include <sys/syscall.h>

extern "C"
source::Value
syscall_write(std::vector<source::Value> args)
{
    if (args.size() < 2) {
        std::cout << "need 2 arguments" << std::endl;
        return source::Value(false);
    }

    int fd = args.at(0).Int();
    std::string buff = args.at(1).Str();
    int retval = syscall(SYS_write, fd, buff.c_str(), buff.size());

    return source::Value(retval);   
}

extern "C"
source::Value
syscall_sync(std::vector<source::Value> args)
{
    if (args.size() < 1) {
        std::cout << "need 1 arguments" << std::endl;
        return source::Value(false);
    }

    int fd = args.at(0).Int();
    int retval = syscall(SYS_sync, fd);

    return source::Value(retval);   
}