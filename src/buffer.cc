#include <buffer.h>

source::out::buffer
::buffer(int fd, int color)
{
    if (fd == 1) dest = std::cout.rdbuf();
    else if (fd == 2) dest = std::cerr.rdbuf();
    else {
        std::cerr << "err[buffer]: file description in StreamBuffer constructor" << std::endl;
        exit(11);
    }

    tty = isatty(fd);

    magic[0] = '\033';
    magic[1] = '[';
    magic[2] = color / 10 + '0';
    magic[3] = color % 10 + '0';
    magic[4] = ';';
    magic[5] = '1';
    magic[6] = 'm';
}

bool
source::out::buffer
::redirect(std::string fname)
{
    if (!fileout.open(fname.c_str(), std::ios_base::out | std::ios_base::trunc)) return false;
    tty  = false;
    dest = &fileout;
    return true;    
}