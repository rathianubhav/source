#ifndef __IO__
#define __IO__

#include <string>
#include <iostream>

namespace color {
    enum code_t {
        BLACK = 30, RED, GREEN,
        YELLOW, BLUE, MAGENTA, CYAN,
        WHITE,
    };

    class obj {
        public:
            static std::string code(code_t c, bool bg = false) {
                int a = (int)c;
                if (bg) a += 10;
                return "\033[1;" + std::to_string(a) + "m";
            }

            static std::string reset() {
                return "\033[0m";
            }

            static std::string bold() {
                return "\033[1m";
            }

            static  std::string underline() {
                return "\033[4m";
            }
    };
    
};

namespace io {

    template<typename ...Args>
    void fprint(std::ostream& o, Args&& ...args)
    {
        (
            o << ... << std::forward<Args>(args)
        );
    }

    template<typename ...Args>
    void print(Args&& ...args)
    {
        return fprint(std::cout, args...);
    }

    template<typename ...Args>
    void println(Args&& ...args)
    {
        return fprint(std::cout, args...,"\n");
    }

    template<typename ...Args>
    const std::string sprint(Args&& ...args)
    {
        std::stringstream ss;
        fprint(ss, args...);
        return ss.str();
    }

    template<typename ...Args>
    void __color_tited(const std::string& code, const std::string& mesg ,Args&& ...args)
    {
        return fprint(std::cerr,code, mesg, color::obj::reset(),": ",color::obj::bold(), args..., color::obj::reset(),"\n");
    }

    template<typename ...Args>
    void error(Args&& ...args)
    {
        return __color_tited(color::obj::code(color::RED), "[Error]", args...);
    }

    template<typename ...Args>
    void success(Args&& ...args)
    {
        return __color_tited(color::obj::code(color::GREEN), "[Success]", args...);
    }

    template<typename ...Args>
    void process(Args&& ...args)
    {
        return __color_tited(color::obj::code(color::CYAN), "[Process]", args...);
    }

    template<typename ...Args>
    void info(Args&& ...args)
    {
        return __color_tited(color::obj::code(color::BLUE), "[Info]", args...);
    }

    template<typename ...Args>
    void warn(Args&& ...args)
    {
        return __color_tited(color::obj::code(color::YELLOW), "[warn]", args...);
    }

    template<typename ...Args>
    void task(int __cur, int __total, Args&& ...args)
    {
        return __color_tited(color::obj::code(color::BLUE), sprint("[",__cur,"/",__total,"]"), args...);
    }

    template<typename ...Args>
    void log(Args&& ...args)
    {
        time_t now = time(0);
        std::string dt = std::string(ctime(&now));
        return fprint(std::cout,"[",dt.substr(0, dt.length() - 1),"] : ", args...,"\n");
    }

}


#ifdef DEBUG
    #define debug(...) io::log(__VA_ARGS__)
#else
    #define debug(...)
#endif

#endif