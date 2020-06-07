#ifndef __BUFFER__
#define __BUFFER__

#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

class Buffer : public std::streambuf {

private:
    std::streambuf *dest;
    std::filebuf fout;
    int color;
    bool tty;
    std::string color_code;

public:
    Buffer(int fd, int color) {
    color(color) {
        switch (fd) {
            case 1: dest = std::cout.rdbuf(); break;
            case 2: dest = std::cerr.rdbuf(); break;
        }

        tty = isatty(fd);

        color_code = "\033[" + (color / 10 + '0') + (color % 10 + '0') + ";1m";
    }

    bool redirect(const std::string &filename) {
        if (!fout.open(filename.c_str(), std::ios_base::out | std::ios_base::trunc))    return false;
        tty = false;
        dest = &fout;

        return true;
    }


protected:
    virtual int overflow(int ch) {
        if (tty) dest->sputn(color_code.c_str(), 7);
        dest->sputc(ch);
        if (tty) dest->sputn("\033[0m", 4);
        return 1;
    }

};

#endif