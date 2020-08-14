#include <bits/stdc++.h>
#include <source/parser.hh>
#include <source/context.hh>
#include <releax/releax>


using namespace std;
using namespace source;
using namespace releax;

int debug = 0;


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

int
parse_func(releax::cli& app)
{
    context::obj cc;

    auto d = app.val_of_arg("debug");
    if (d.length() != 0) debug = atoi(d.c_str());

    if (app.args.size() == 0) {
        while(true) {
            string input;
            cout << ">> ";
            getline(cin, input);

            auto l = make_unique<lexer::obj>(input);
            auto p = make_unique<parser::obj>(move(l), cc);
            p->parse();
        }
    } else {
        auto input = readfile(app.args.at(0));
        auto l = make_unique<lexer::obj>(input);
        auto p = make_unique<parser::obj>(move(l), cc);
        p->parse();

    }
    return 0;
}

int
lexer_func(releax::cli& app)
{
    context::obj cc;
    
    auto d = app.val_of_arg("debug");
    if (d.length() != 0) debug = atoi(d.c_str());

    if (app.args.size() == 0) {
        while(true) {
            string input;
            cout << ">> ";
            getline(cin, input);

            auto l = make_unique<lexer::obj>(input);
            
            token::obj curtok;
            do {
                curtok = l->next_token();
                cout << "[" << curtok.get_type() << ":" << curtok.get_lit() << "]" << endl;
            } while(curtok.get_type() != token::TEOF);
        }
    } else {
        auto input = readfile(app.args.at(0));
        auto l = make_unique<lexer::obj>(input);
            
        token::obj curtok;
        do {
            curtok = l->next_token();
            cout << "[" << curtok.get_type() << ":" << curtok.get_lit() << "]" << endl;
        } while(curtok.get_type() != token::TEOF);

    }
    return 0;
}


int
main(int ac, char** av)
{
    releax::cli app;
    app.name("source")
       .version(0.1)
       .release('a')
       .description("a simple yet efficient programming language")
       .author("Manjeet Singh", "itsmanjeet1998@gmail.com","-")
       .sub("lex","start lexical analyiser", "[filename]", lexer_func)
       .sub("parse","start parser", "[filename]", parse_func);

    return app.execute(ac, av);
}