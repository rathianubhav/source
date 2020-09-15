#include <source/io.hh>

using namespace source;

void
ver_info() {

    io::info("Language Source");
    io::info("Version 0.0.0.0 a (In development)");
    io::info("Currently working on lexer :)");
    io::info("Notes : review const in lexer and skipped double, enum, float, long, short, signed, struct");
    io::info("Notes : Lexical testing passed :)");
}