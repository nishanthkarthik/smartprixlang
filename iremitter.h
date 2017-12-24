#ifndef IREMITTER_H
#define IREMITTER_H

#include "emitter.h"

#include <unordered_map>

class IREmitter : public Emitter {
    unordered_map<string, int> registers;

public:
    IREmitter(ostream* o, vector<Instruction>& ins);
};

#endif // IREMITTER_H
