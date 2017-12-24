#ifndef CTRANSPILER_H
#define CTRANSPILER_H

#include "emitter.h"

class Transpiler : public Emitter {
    typedef void (Transpiler::*memfuncptr)(Instruction& i);
    unordered_map<string, memfuncptr> c_map;

public:
    Transpiler(ostream* o, vector<Instruction>& ins);
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

#endif // CTRANSPILER_H
