#include "ctranspiler.h"

Transpiler::Transpiler(ostream* o, vector<Instruction>& ins)
    : Emitter(o, ins)
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
    parse_names();
}

void Transpiler::transpile_c()
{
    emit("#include <stdlib.h>");
    emit("#include <stdio.h>");
    emit("int main() {");
    for (auto it = varnames.begin(); it != varnames.end(); ++it)
        emit("int " + *it + ";");
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
    emit("printf(\"%d\\n\", " + i.arg_at(0).get_name() + ");");
}

void Transpiler::transpile_c_goto(Instruction& i)
{
    emit("goto " + i.arg_at(0).get_name() + ";");
}

void Transpiler::transpile_c_label(Instruction& i)
{
    emit(i.arg_at(0).get_name() + ":");
}

void Transpiler::transpile_c_set(Instruction& i)
{
    emit(i.arg_at(0).get_name() + " = " + i.arg_at(1).get_name() + ";");
}

void Transpiler::transpile_c_add(Instruction& i)
{
    emit(i.arg_at(0).get_name() + " = " + i.arg_at(1).get_name() + " + " + i.arg_at(2).get_name() + ";");
}

void Transpiler::transpile_c_sub(Instruction& i)
{
    emit(i.arg_at(0).get_name() + " = " + i.arg_at(1).get_name() + " - " + i.arg_at(2).get_name() + ";");
}

void Transpiler::transpile_c_mul(Instruction& i)
{
    emit(i.arg_at(0).get_name() + " = " + i.arg_at(1).get_name() + " * " + i.arg_at(2).get_name() + ";");
}

void Transpiler::transpile_c_div(Instruction& i)
{
    emit(i.arg_at(0).get_name() + " = " + i.arg_at(1).get_name() + " / " + i.arg_at(2).get_name() + ";");
}

void Transpiler::transpile_c_if(Instruction& i)
{
    emit("if (" + i.arg_at(0).get_name() + i.arg_at(1).get_name() + i.arg_at(2).get_name() + ") {");
}
