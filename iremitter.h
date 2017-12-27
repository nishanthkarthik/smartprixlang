#ifndef IREMITTER_H
#define IREMITTER_H

#include "emitter.h"

#include <stack>
#include <unordered_map>

class IREmitter : public Emitter {
    typedef void (IREmitter::*memfuncptr)(Instruction i);
    int regcnt, condcnt;
    unordered_map<string, memfuncptr> ir_fmap;
    stack<int> condstack;

public:
    IREmitter(ostream* o, vector<Instruction>& ins);

    void ir_emit();

    void ir_setup();
    void ir_start_main();
    void ir_end_main();
    void ir_var_alloc();

    void ir_exit(Instruction i);
    void ir_endif(Instruction i);
    void ir_echo(Instruction i);
    void ir_goto(Instruction i);
    void ir_label(Instruction i);
    void ir_set(Instruction i);
    void ir_binop(Instruction i, string opinst);
    void ir_add(Instruction i);
    void ir_sub(Instruction i);
    void ir_mul(Instruction i);
    void ir_div(Instruction i);
    void ir_if(Instruction i);
};

#endif // IREMITTER_H
