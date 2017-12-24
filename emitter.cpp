#include "emitter.h"
#include "parser.h"

Emitter::Emitter(ostream* o, vector<Instruction>& ins)
    : outstr(o)
    , insvec(ins)
{
}

void Emitter::error(string errmsg, Instruction& i)
{
    cerr << i << " -> " << errmsg << endl;
}

void Emitter::emit(string msg)
{
    *outstr << msg << endl;
}

void Emitter::parse_names()
{
    for (Instruction& i : insvec)
        for (Term& t : i.get_args())
            if (t.get_type() == TT_VARIABLE and i.get_cmd() != "LABEL" and i.get_cmd() != "GOTO")
                varnames.insert(t.get_name());
            else if (t.get_type() == TT_VARIABLE)
                labels.insert(t.get_name());
}

bool Emitter::is_arglen_valid()
{
    bool t = true;
    for (Instruction& i : insvec) {
        string cmd = i.get_cmd();
        if (ArgLen.find(cmd) == ArgLen.end()) {
            t = t and false;
            error("Unknown command", i);
        } else if (ArgLen.at(cmd) != i.get_args().size()) {
            t = t and false;
            error("Argument length mismatch", i);
        }
    }
    return t;
}
