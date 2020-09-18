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
                token::obj(token::num, "5"),
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
                token::obj(token::ident, "x"),
                token::obj(token::num, "5"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing Print");
    } catch (std::runtime_error e) {
        io::error("Testing Print");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "auto x = 5;",
            vector<token::obj> {
                token::obj(token::__auto, "auto"),
                token::obj(token::ident, "x"),
                token::obj(token::assign, "="),
                token::obj(token::num, "5"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing auto");
    } catch (std::runtime_error e) {
        io::error("Testing auto");
        io::info(e.what());
    }
    
    try {
        lexer::obj::__test__(
            "break;",
            vector<token::obj> {
                token::obj(token::__break, "break"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing break");
    } catch (std::runtime_error e) {
        io::error("Testing break");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "case a: x = 5;",
            vector<token::obj> {
                token::obj(token::__case, "case"),
                token::obj(token::ident,"a"),
                token::obj(token::colon,":"),
                token::obj(token::ident, "x"),
                token::obj(token::assign, "="),
                token::obj(token::num, "5"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing case");
    } catch (std::runtime_error e) {
        io::error("Testing case");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "char x;",
            vector<token::obj> {
                token::obj(token::__char, "char"),
                token::obj(token::ident, "x"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing char");
    } catch (std::runtime_error e) {
        io::error("Testing char");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "char const x = \"hi\";",
            vector<token::obj> {
                token::obj(token::__char, "char"),
                token::obj(token::_const, "const"),
                token::obj(token::ident, "x"),
                token::obj(token::assign, "="),
                token::obj(token::__string, "hi"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing const");
    } catch (std::runtime_error e) {
        io::error("Testing const");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "continue;",
            vector<token::obj> {
                token::obj(token::__continue, "continue"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing continue");
    } catch (std::runtime_error e) {
        io::error("Testing continue");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "default : x = 5;",
            vector<token::obj> {
                token::obj(token::__default, "default"),
                token::obj(token::colon,":"),
                token::obj(token::ident, "x"),
                token::obj(token::assign, "="),
                token::obj(token::num, "5"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing default");
    } catch (std::runtime_error e) {
        io::error("Testing default");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "extern char x = \"hi\";",
            vector<token::obj> {
                token::obj(token::__extern,"extern"),
                token::obj(token::__char, "char"),
                token::obj(token::ident, "x"),
                token::obj(token::assign, "="),
                token::obj(token::__string, "hi"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing extern");
    } catch (std::runtime_error e) {
        io::error("Testing extern");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "goto jbl",
            vector<token::obj> {
                token::obj(token::__goto, "goto"),
                token::obj(token::ident, "jbl"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing goto");
    } catch (std::runtime_error e) {
        io::error("Testing goto");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "register int x = 5 ;",
            vector<token::obj> {
                token::obj(token::__register, "register"),
                token::obj(token::ident, "int"),
                token::obj(token::ident, "x"),
                token::obj(token::assign, "="),
                token::obj(token::num, "5"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing register");
    } catch (std::runtime_error e) {
        io::error("Testing register");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "return x;",
            vector<token::obj> {
                token::obj(token::__return, "return"),
                token::obj(token::ident, "x"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing return");
    } catch (std::runtime_error e) {
        io::error("Testing return");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "sizeof(char);",
            vector<token::obj> {
                token::obj(token::__sizeof, "sizeof"),
                token::obj(token::lbrace,"("),
                token::obj(token::__char, "char"),
                token::obj(token::rbrace, ")"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing sizeof");
    } catch (std::runtime_error e) {
        io::error("Testing sizeof");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "static char x = \"hi\";",
            vector<token::obj> {
                token::obj(token::__static,"static"),
                token::obj(token::__char, "char"),
                token::obj(token::ident, "x"),
                token::obj(token::assign, "="),
                token::obj(token::__string, "hi"),
                token::obj(token::semicolon, ";")
            }
        );
        io::success("Testing static");
    } catch (std::runtime_error e) {
        io::error("Testing static");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "switch(dir) {case 14 : print(\"Up\"); break; case 21 : print(\"Down\"); break; default : print(\"Stop\"); }" ,
            vector<token::obj> {
                token::obj(token::__switch,"switch"),
                token::obj(token::lbrace, "("),
                token::obj(token::ident, "dir"),
                token::obj(token::rbrace, ")"),
                token::obj(token::lparen, "{"),
                token::obj(token::__case,"case"),
                token::obj(token::num,"14"),
                token::obj(token::colon,":"),
                token::obj(token::__print,"print"),
                token::obj(token::lbrace,"("),
                token::obj(token::__string,"Up"),
                token::obj(token::rbrace,")"),
                token::obj(token::semicolon,";"),
                token::obj(token::__break,"break"),
                token::obj(token::semicolon, ";"),
                token::obj(token::__case,"case"),
                token::obj(token::num,"21"),
                token::obj(token::colon,":"),
                token::obj(token::__print,"print"),
                token::obj(token::lbrace,"("),
                token::obj(token::__string,"Down"),
                token::obj(token::rbrace,")"),
                token::obj(token::semicolon,";"),
                token::obj(token::__break,"break"),
                token::obj(token::semicolon, ";"),
                token::obj(token::__default,"default"),
                token::obj(token::colon,":"),
                token::obj(token::__print,"print"),
                token::obj(token::lbrace,"("),
                token::obj(token::__string,"Stop"),
                token::obj(token::rbrace,")"),
                token::obj(token::semicolon,";"),
                token::obj(token::rparen,"}")
            }
        );
        io::success("Testing switch");
    } catch (std::runtime_error e) {
        io::error("Testing switch");
        io::info(e.what());
    }

    try {
        lexer::obj::__test__(
            "5123 598.64 5.2.6.4",
            vector<token::obj> {
                token::obj(token::num, "5123"),
                token::obj(token::decimal, "598.64"),
                token::obj(token::str, "5.2.6.4")
            }
        );
        io::success("Testing Number type");
    } catch (std::runtime_error e) {
        io::error("Testing Number type");
        io::info(e.what());
    }
}