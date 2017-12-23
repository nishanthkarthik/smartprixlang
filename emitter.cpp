#include "emitter.h"
#include "parser.h"

void Transpiler::error(string errmsg, Instruction& i)
{
    cerr << i << " -> " << errmsg << endl;
}

void Transpiler::emit(string msg)
{
    *outstr << msg << endl;
}

Transpiler::Transpiler(ostream* o, vector<Instruction>& ins)
    : outstr(o)
    , insvec(ins)
{
    c_map.emplace("EXIT", &Transpiler::transpile_c_exit);
    c_map.emplace("ENDIF", &Transpiler::transpile_c_endif);
    c_map.emplace("ECHO", &Transpiler::transpile_c_echo);
    c_map.emplace("GOTO", &Transpiler::transpile_c_goto);
    c_map.emplace("LABEL", &Transpiler::transpile_c_label);
    c_map.emplace("SET", &Transpiler::transpile_c_set);
    c_map.emplace("ADD", &Transpiler::transpile_c_add);
    c_map.emplace("SUB", &Transpiler::transpile_c_sub);
    c_map.emplace("MUL", &Transpiler::transpile_c_mul);
    c_map.emplace("DIV", &Transpiler::transpile_c_div);
    c_map.emplace("IF", &Transpiler::transpile_c_if);
    is_arglen_valid();
}

unordered_set<string> Transpiler::get_varnames()
{
    unordered_set<string> r;
    for (Instruction& i : insvec)
        for (Term& t : i.get_args())
            if (t.get_type() == TT_VARIABLE)
                r.insert(t.get_name());
    return r;
}

bool Transpiler::is_arglen_valid()
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

void Transpiler::transpile_c()
{
    emit("#include <stdlib.h>");
    emit("#include <stdio.h>");
    emit("int main() {");
    unordered_set<string> varnames = get_varnames();
    for (auto it = varnames.begin(); it != varnames.end(); ++it)
        emit("double " + *it + ";");
    for (Instruction& i : insvec) {
        string cmd = i.get_cmd();
        if (c_map.find(cmd) != c_map.end()) {
            memfuncptr m = c_map.at(cmd);
            (this->*m)(i);
        }
    }
    emit("return 0;");
    emit("}");
}

void Transpiler::transpile_c_exit(Instruction& i)
{
    emit("exit(0);");
}

void Transpiler::transpile_c_endif(Instruction& i)
{
    emit("}");
}

void Transpiler::transpile_c_echo(Instruction& i)
{
    emit("printf(\"%g\\n\", " + i.get_args().front().get_name() + ");");
}

void Transpiler::transpile_c_goto(Instruction& i)
{
    emit("goto " + i.get_args().front().get_name() + ";");
}

void Transpiler::transpile_c_label(Instruction& i)
{
    emit(i.get_args().front().get_name() + ":");
}

void Transpiler::transpile_c_set(Instruction& i)
{
    auto args = i.get_args();
    emit(args[0].get_name() + " = " + args[1].get_name() + ";");
}

void Transpiler::transpile_c_add(Instruction& i)
{
    auto args = i.get_args();
    emit(args[0].get_name() + " = " + args[1].get_name() + " + " + args[2].get_name() + ";");
}

void Transpiler::transpile_c_sub(Instruction& i)
{
    auto args = i.get_args();
    emit(args[0].get_name() + " = " + args[1].get_name() + " - " + args[2].get_name() + ";");
}

void Transpiler::transpile_c_mul(Instruction& i)
{
    auto args = i.get_args();
    emit(args[0].get_name() + " = " + args[1].get_name() + " * " + args[2].get_name() + ";");
}

void Transpiler::transpile_c_div(Instruction& i)
{
    auto args = i.get_args();
    emit(args[0].get_name() + " = " + args[1].get_name() + " / " + args[2].get_name() + ";");
}

void Transpiler::transpile_c_if(Instruction& i)
{
    auto args = i.get_args();
    emit("if (" + args[0].get_name() + args[1].get_name() + args[2].get_name() + ") {");
}
