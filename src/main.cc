#include <bits/stdc++.h>
#include <source/parser.hh>
#include <source/context.hh>

using namespace std;
using namespace source;

int debug = 0;

enum MODE {
    LEXER,
    LABEL,
    FULL,
    TEST_LEXER,
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

        do {
            cout << ">> ";
            getline(std::cin, input);
            lexer::obj __lexer__(input);

            if (m == LEXER) {
                auto _t = __lexer__.next_token();
                while(_t.get_type() != token::eof) {
                    cout << "[" << _t.get_lit() << ":" << _t.get_type() << "]" << endl;
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
                        cout << i->get()->label() << endl;
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
main(int ac, char** av)
{
    bool interactive_mode = (ac <= 1);
    MODE __mode__ = FULL;
    std::string __file_name__;

    for(int i = 1; i < ac; i++) {
        std::string args(av[i]);
        if (args[0] == '-') {
            if (args.substr(1, args.length() - 1) == "lexer") __mode__ = LEXER;
            else if (args.substr(1, args.length() - 1) == "label") __mode__ = LABEL;
            else if (args.substr(1, args.length() - 1) == "test-lexer") __mode__ = TEST_LEXER;
            else 
                throw std::runtime_error("Invalid argument");
        } else {
            __file_name__ = args;
            interactive_mode = false;
        }
    }

    if (__mode__ == TEST_LEXER) {

        test_lexer();
    } else {
        start_engine(interactive_mode, __mode__);
    }
    
}