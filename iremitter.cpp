#include "iremitter.h"
#include "parser.h"

inline string regtos(int regidx)
{
    return "%" + to_string(regidx);
}

IREmitter::IREmitter(ostream* o, vector<Instruction>& ins)
    : Emitter(o, ins)
{
    regcnt = 1;
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

void IREmitter::echo(const string s)
{
    *outstr << s << endl;
}

void IREmitter::ir_setup()
{
    echo("target triple = \"x86_64-unknown-linux-gnu\"");
    echo("declare i32 @printf(i8*, ...)");
    echo("@decstr = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\"");
}

void IREmitter::ir_start_main()
{
    echo("define i32 @main() {");
}

void IREmitter::ir_end_main()
{
    echo("ret i32 0");
    echo("}");
}

void IREmitter::ir_emit()
{
    emit("");
    ir_setup();
    emit("");
    ir_start_main();
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
    echo("ret i32 1");
}

void IREmitter::ir_endif(Instruction i)
{
}

void IREmitter::ir_echo(Instruction i)
{
    regcnt++;
    emit("call i32 (i8*, ...) @printf(i8* getelementptr inbounds "
         "([4 x i8], [4 x i8]* @decstr, i32 0, i32 0), i32 "
        + regtos(registers[i.arg_at(0).get_name()]) + ")");
}

void IREmitter::ir_goto(Instruction i)
{
    emit("br label %" + i.arg_at(0).get_name());
}

void IREmitter::ir_label(Instruction i)
{
    emit("br label %" + i.arg_at(0).get_name());
    emit(i.arg_at(0).get_name() + ":");
}

void IREmitter::ir_set(Instruction i)
{
    if (registers.find(i.arg_at(0).get_name()) == registers.end())
        registers[i.arg_at(0).get_name()] = regcnt++;
    switch (i.arg_at(1).get_type()) {
    case TT_VARIABLE:
        echo(regtos(registers[i.arg_at(0).get_name()]) + " = add i32 0, " + regtos(registers[i.arg_at(1).get_name()]));
        break;
    case TT_VALUE:
        echo(regtos(registers[i.arg_at(0).get_name()]) + " = add i32 0, " + to_string(i.arg_at(1).get_value()));
        break;
    default:
        break;
    }
}

void IREmitter::ir_binop(Instruction i, string opinst)
{
    string rhs1, rhs2;

    switch (i.arg_at(1).get_type()) {
    case TT_VALUE:
        rhs1 = to_string(i.arg_at(1).get_value());
        break;
    case TT_VARIABLE:
        rhs1 = regtos(registers[i.arg_at(1).get_name()]);
        break;
    default:
        break;
    }

    switch (i.arg_at(2).get_type()) {
    case TT_VALUE:
        rhs2 = to_string(i.arg_at(2).get_value());
        break;
    case TT_VARIABLE:
        rhs2 = regtos(registers[i.arg_at(2).get_name()]);
        break;
    default:
        break;
    }

    registers[i.arg_at(0).get_name()] = regcnt++;
    string lhs = regtos(registers[i.arg_at(0).get_name()]);

    echo(lhs + " = " + opinst + " i32 " + rhs1 + ", " + rhs2);
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
}
