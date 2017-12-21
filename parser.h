#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <vector>

using namespace std;

enum Keyword {
    ECHO,
    EXIT,
    SET,
    ADD,
    SUB,
    MUL,
    DIV,
    GOTO,
    LABEL,
    IF,
    ENDIF
};

class Expr {
    int nargs;
    vector<string> args;

public:
    Expr(int n, vector<string>& v)
        : nargs(n)
        , args(v)
    {
    }
};

class Parser {
    istream* inst;
    ostream* outst;
    vector<Expr> code;

public:
    Parser(istream* i, ostream* o)
        : inst(i)
        , outst(o)
    {
    }

    void parse();
    void parse_echo();
    void parse_set();
    void parse_op();
    void parse_goto();
    void parse_label();
    void parse_cond();
    void parse_exit();
};

#endif // PARSER_H
