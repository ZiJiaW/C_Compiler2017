#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "Lexer.h"
class Parser {
public:
    Parser(Lexer &_lex, ErrorHandler &_eh);
private:
    Lexer &lex;
    ErrorHandler &eh;
};

#endif // PARSER_H_INCLUDED
