#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>

namespace source {
namespace out {

class buffer : public std::streambuf {
private:
    std::streambuf* dest;
    std::filebuf fileout;
    bool tty;
    char magic[7];

public:
    buffer(int fd, int color);

    bool redirect(std::string fname);

protected:
    virtual int overflow(int ch) {
        if (tty) dest->sputn(magic, 7);
        dest->sputc(ch);
        if (tty) dest->sputn("\033[0m", 4);
        return 1;
    }
};


class stream : public std::ostream {
private:
    buffer buf;
public:
    stream(int fd, int cc) : std::ostream(&buf), buf(fd, cc) {}

    bool redirect(std::string fname) {
        return buf.redirect(fname.c_str());
    }
};

}
}

