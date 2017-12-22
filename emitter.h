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

class Transpiler {
    typedef void (Transpiler::*memfuncptr)(Instruction& i);

    ostream* outstr;
    vector<Instruction> insvec;
    unordered_map<string, memfuncptr> c_map;

public:
    Transpiler(ostream* o, vector<Instruction>& ins);

    bool is_arglen_valid();
    void error(string errmsg, Instruction& i);
    void emit(string msg);

    unordered_set<string> get_varnames();

    void transpile_c();
    void transpile_c_exit(Instruction& i);
    void transpile_c_endif(Instruction& i);
    void transpile_c_echo(Instruction& i);
    void transpile_c_goto(Instruction& i);
    void transpile_c_label(Instruction& i);
    void transpile_c_set(Instruction& i);
    void transpile_c_add(Instruction& i);
    void transpile_c_sub(Instruction& i);
    void transpile_c_mul(Instruction& i);
    void transpile_c_div(Instruction& i);
    void transpile_c_if(Instruction& i);
};

#endif // EMITTER_H
