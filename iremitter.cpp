#include "iremitter.h"

inline string regtos(int regidx)
{
    return "%t" + to_string(regidx);
}

inline string iftos(int i, string key, string st = "")
{
    return st + key + to_string(i);
}

IREmitter::IREmitter(ostream* o, vector<Instruction>& ins)
    : Emitter(o, ins)
{
    condcnt = regcnt = 1;
    ir_fmap.emplace("EXIT", &IREmitter::ir_exit);
    ir_fmap.emplace("ENDIF", &IREmitter::ir_endif);
    ir_fmap.emplace("ECHO", &IREmitter::ir_echo);
    ir_fmap.emplace("GOTO", &IREmitter::ir_goto);
    ir_fmap.emplace("LABEL", &IREmitter::ir_label);
    ir_fmap.emplace("SET", &IREmitter::ir_set);
    ir_fmap.emplace("ADD", &IREmitter::ir_add);
    ir_fmap.emplace("SUB", &IREmitter::ir_sub);
    ir_fmap.emplace("MUL", &IREmitter::ir_mul);
    ir_fmap.emplace("DIV", &IREmitter::ir_div);
    ir_fmap.emplace("IF", &IREmitter::ir_if);
    is_arglen_valid();
    parse_names();
}

void IREmitter::ir_setup()
{
    emit("target triple = \"x86_64-unknown-linux-gnu\"");
    emit("declare i32 @printf(i8*, ...)");
    emit("@decstr = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\"");
}

void IREmitter::ir_var_alloc()
{
    for (auto it = varnames.begin(); it != varnames.end(); ++it)
        emit("%" + *it + " = alloca i32");
}

void IREmitter::ir_start_main()
{
    emit("define i32 @main() {");
}

void IREmitter::ir_end_main()
{
    emit("ret i32 0");
    emit("}");
}

void IREmitter::ir_emit()
{
    emit("");
    ir_setup();
    emit("");
    ir_start_main();
    emit("");
    ir_var_alloc();
    emit("");
    for (Instruction i : insvec) {
        string cmd = i.get_cmd();
        if (ir_fmap.find(cmd) != ir_fmap.end()) {
            memfuncptr m = ir_fmap[cmd];
            (this->*m)(i);
        }
    }
    ir_end_main();
    emit("");
}

void IREmitter::ir_exit(Instruction i)
{
    emit("ret i32 1");
}

void IREmitter::ir_echo(Instruction i)
{
    string treg = regtos(regcnt++);
    emit(treg + " = load i32, i32* %" + i.name_at(0));
    emit("call i32 (i8*, ...) @printf(i8* getelementptr inbounds "
         "([4 x i8], [4 x i8]* @decstr, i32 0, i32 0), i32 "
        + treg + ")");
}

void IREmitter::ir_goto(Instruction i)
{
    emit("br label %" + i.name_at(0));
}

void IREmitter::ir_label(Instruction i)
{
    emit("br label %" + i.name_at(0));
    emit(i.name_at(0) + ":");
}

void IREmitter::ir_set(Instruction i)
{
    string treg;
    switch (i.arg_at(1).get_type()) {
    case TT_VARIABLE:
        treg = regtos(regcnt++);
        emit(treg + " = " + "load i32, i32* %" + i.name_at(1));
        emit("store i32 " + treg + ", i32* %" + i.name_at(0));
        break;
    case TT_VALUE:
        emit("store i32 " + to_string(i.val_at(1)) + ", i32* %" + i.name_at(0));
        break;
    default:
        break;
    }
}

void IREmitter::ir_binop(Instruction i, string opinst)
{
    string rhs1 = to_string(i.val_at(1));
    string rhs2 = to_string(i.val_at(2));
    if (i.arg_at(1).get_type() == TT_VARIABLE) {
        rhs1 = regtos(regcnt++);
        emit(rhs1 + " = load i32, i32* %" + i.name_at(1));
    }
    if (i.arg_at(2).get_type() == TT_VARIABLE) {
        rhs2 = regtos(regcnt++);
        emit(rhs2 + " = load i32, i32* %" + i.name_at(2));
    }
    string resreg = regtos(regcnt++);
    emit(resreg + " = " + opinst + " i32 " + rhs1 + ", " + rhs2);
    emit("store i32 " + resreg + ", i32* %" + i.name_at(0));
}

void IREmitter::ir_add(Instruction i)
{
    ir_binop(i, "add");
}

void IREmitter::ir_sub(Instruction i)
{
    ir_binop(i, "sub");
}

void IREmitter::ir_mul(Instruction i)
{
    ir_binop(i, "mul");
}

void IREmitter::ir_div(Instruction i)
{
    ir_binop(i, "udiv");
}

void IREmitter::ir_if(Instruction i)
{
    static unordered_map<string, string> relmap = {
        { "<", "slt" },
        { ">", "sgt" },
        { "==", "eq" },
        { "!=", "ne" }
    };
    string t1 = to_string(i.val_at(0));
    string t2 = to_string(i.val_at(2));
    if (i.arg_at(0).get_type() == TT_VARIABLE) {
        t1 = regtos(regcnt++);
        emit(t1 + " = load i32, i32* %" + i.name_at(0));
    }
    if (i.arg_at(2).get_type() == TT_VARIABLE) {
        t2 = regtos(regcnt++);
        emit(t2 + " = load i32, i32* %" + i.name_at(2));
    }
    string cmpres = regtos(regcnt++);
    int cmp_idx = condcnt++;
    emit(cmpres + " = icmp " + relmap[i.name_at(1)] + " i32 " + t1 + ", " + t2);
    emit("br i1 " + cmpres
        + ", label " + iftos(cmp_idx, "beg", "%")
        + ", label " + iftos(cmp_idx, "end", "%"));

    condstack.push(cmp_idx);
    emit(iftos(cmp_idx, "beg") + ":");
}

void IREmitter::ir_endif(Instruction i)
{
    int cmp_idx = condstack.top();
    condstack.pop();
    emit("br label " + iftos(cmp_idx, "ret", "%"));
    emit(iftos(cmp_idx, "end") + ":");
    emit("br label " + iftos(cmp_idx, "ret", "%"));
    emit(iftos(cmp_idx, "ret") + ":");
}