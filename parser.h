#ifndef PARSER_H
#define PARSER_H

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <locale>
#include <string>
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

enum Termtype {
    TT_VARIABLE,
    TT_VALUE,
    TT_RELOP,
    TT_UNDEF
};

class Term {
    int value;
    string name;
    Termtype type;

public:
    Term(string& nm);
    friend ostream& operator<<(ostream& os, const Term& r);
    Termtype get_type();
    int get_value();
    string get_name();
};

class Instruction {
    string cmd;
    vector<Term> args;

public:
    Instruction(string& c, vector<Term>& v);
    friend ostream& operator<<(ostream& os, const Instruction& r);
    string get_cmd() const;
    vector<Term> get_args() const;
    Term arg_at(int i) const;
};

class Parser {
    istream* instr;
    vector<Instruction> code;

public:
    Parser(istream* i)
        : instr(i)
    {
    }

    vector<Instruction> parse();
};

#endif // PARSER_H
