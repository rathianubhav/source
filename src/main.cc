#include <bits/stdc++.h>
#include <source/parser.hh>
#include <source/context.hh>
#include <releax/releax>

using namespace std;
using namespace source;
using namespace releax;

int debug = 0;

enum MODE {
    LEXER,
    LABEL,
    FULL
};

string
readfile(const string& file)
{
    ifstream fptr(file, ios::in | ios::binary | ios::ate);

    ifstream::pos_type file_size = fptr.tellg();
    fptr.seekg(0, ios::beg);
    vector<char> buff(file_size);
    fptr.read(buff.data(), file_size);

    return string(buff.data(), file_size);
}

void
start_engine(bool interactive, MODE m = FULL)
{
    context::obj cc;
    string input;

    string __mode_mesg = "";
    switch (m) {
        case LEXER:
            __mode_mesg = "(Lexer analysis)";
            break;
        
        case LABEL:
            __mode_mesg = "(Label analysis)";
            break;
    }

    if (interactive) {
        io::info("Interactive Mode ", __mode_mesg);
        io::info("Source shell, input statment to execute");

        do {
            io::print(">> ");
            getline(std::cin, input);
            lexer::obj __lexer__(input);

            if (m == LEXER) {
                auto _t = __lexer__.next_token();
                while(_t.get_type() != token::eof) {
                    io::println("[",_t.get_lit(),":",_t.get_type(),"]");
                    _t = __lexer__.next_token();
                }
            } else {
                parser::obj __parser__(__lexer__,cc);
                auto __prog__ = __parser__.parse();
                if (m == LABEL) {
                    for(auto i = __prog__->stmts.begin();
                         i != __prog__->stmts.end();
                         i++)
                    {
                        io::info(i->get()->label());
                    }
                } else {
                    for(auto i = __prog__->stmts.begin();
                         i != __prog__->stmts.end();
                         i++)
                    {
                        i->get()->exec(cc);
                    }
                }
                
            }
        } while (true);
    }
}

int
lexer_func(app::obj& ref)
{
    start_engine(true, LEXER);
    return 0;
}

int
label_func(app::obj& ref)
{

    start_engine(true, LABEL);


    return 0;
}

int
__main_func(app::obj& ref)
{
    start_engine(true);

    return 0;
}




int
main(int ac, char** av)
{

    app::obj srcapp(ac, av);

    srcapp.id("source")
          .ver("0.1.0")
          .about("a simple and efficient programming language")
          .sub("lex","lexically analysic the source file","[filename]",lexer_func)
          .sub("label","print labels or ast","[filename]",label_func)
          .main(__main_func);

    return srcapp.run().status();
}