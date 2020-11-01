#include <bits/stdc++.h>
#include <source/parser.hh>
#include <source/context.hh>
#include <source/ver_info.hh>

using namespace std;
using namespace source;

int debug = 0;

enum DEBUG {
    DEBUG_LEXER,
    DEBUG_AST,
    DEBUG_PARSER,
};

enum MODE {
    INTERPRETER,
    LEXER,
    LABEL,
    NONE,
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
source_engine(int mode, string file)
{
    context::obj cc;
    do {
        string input;
        if (file.length() == 0) {
            cout << ">> ";
            getline(cin, input);
        }
        else input = readfile(file);

        lexer::obj l(input);
        if (mode == LEXER)
        {
            auto tkn = l.next_token();
            while(tkn.get_type() != token::eof)
                io::println("[", tkn.get_lit(), "]");
        }
        else
        {
            parser::obj p(l, cc);
            auto prog = p.parse();

            if (mode == LABEL)
            {
                for(auto i = prog->stmts.begin();
                         i != prog->stmts.end();
                         i++)
                {
                    i->get()->label();
                }
            } else {
                for(auto i = prog->stmts.begin();
                         i != prog->stmts.end();
                         i++)
                {
                    i->get()->exec(cc);
                }
            }
        }

        if (file.length() != 0)
        {
            mode = NONE;
        }
    } while (mode == INTERPRETER);
}

class cli {
    private:
        int ac;
        char** av;
        vector<string> args, flags;
        map<string, string> values;

    public:
        cli(int ac, char** av)
            : ac(ac), av(av)
        {
            string arg;
            for(int i = 1; i < ac; i++)
            {
                arg = av[i];
                if (av[i][0] == '-')
                {
                    arg = arg.substr(1, arg.length() - 1);
                    if (arg[1] == '-')
                        arg = arg.substr(1, arg.length() - 1);
                    
                    flags.push_back(arg);
                }

                else
                {
                    size_t rdx = arg.find_first_of('=');
                    if (rdx == string::npos)
                    {
                        args.push_back(arg);
                    }

                    else
                    {
                        string var = arg.substr(0, rdx);
                        string val = arg.substr(rdx + 1, arg.length() - (rdx + 1));
                        values.insert(make_pair(var, val));
                    }
                }
            }
        }

        void run()
        {
            for(auto a : values)
            {
                if (a.first == "debug")
                {
                    if (a.second == "lexer")    debug |= DEBUG_LEXER;
                    else if (a.second == "parser") debug |= DEBUG_PARSER;
                    else if (a.second == "ast") debug |= DEBUG_AST;
                }
            }
            int MODE = INTERPRETER;
            string file;
            if (args.size() != 0)
            {
                if (args[0] == "lex")   MODE = LEXER;
                else if (args[0] == "label") MODE = LABEL;
                else {
                    file = args[0];
                }
            }

            source_engine(MODE, file);
        }
};

int
main(int ac, char** av)
{

    cli source_cli(ac, av);

    source_cli.run();    
}