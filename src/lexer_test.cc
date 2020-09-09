#include <source/lexer.hh>
#include <source/io.hh>

using namespace source;

void
test_lexer()
{
    try {
        lexer::obj::__test__(
            "int x = 5;",
            vector<token::obj> {
                token::obj(token::ident, "int"),
                token::obj(token::ident, "x"),
                token::obj(token::assign, "="),
                token::obj(token::__int, "5"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing Assign");
    } catch (std::runtime_error e) {
        io::error("Testing Assign");
        io::info(e.what());
    }


    try {
        lexer::obj::__test__(
            "print x 5;",
            vector<token::obj> {
                token::obj(token::__print, "print"),
                token::obj(token::ident, "asd"),
                token::obj(token::__int, "5"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing Print");
    } catch (std::runtime_error e) {
        io::error("Testing Print");
        io::info(e.what());
    }
    
}