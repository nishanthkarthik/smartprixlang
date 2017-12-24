#ifndef EMITTER_H
#define EMITTER_H

#include "parser.h"
#include <iostream>
#include <unordered_map>
#include <unordered_set>

const unordered_map<string, size_t> ArgLen = {
    { "EXIT", 0 },
    { "ENDIF", 0 },
    { "ECHO", 1 },
    { "GOTO", 1 },
    { "LABEL", 1 },
    { "SET", 2 },
    { "ADD", 3 },
    { "SUB", 3 },
    { "MUL", 3 },
    { "DIV", 3 },
    { "IF", 3 }
};

class Emitter {
protected:
    ostream* outstr;
    vector<Instruction> insvec;
    unordered_set<string> varnames, labels;

public:
    Emitter(ostream* o, vector<Instruction>& ins);

    bool is_arglen_valid();
    void error(string errmsg, Instruction& i);
    void emit(string msg);

    void parse_names();
};

#endif // EMITTER_H
