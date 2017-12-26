#include "ctranspiler.h"
#include "iremitter.h"
#include "parser.h"
#include <iostream>

int main()
{
    Parser p(&cin);
    auto ins = p.parse();
    // Transpiler t(&cout, ins);
    // t.transpile_c();
    IREmitter e(&cout, ins);
    e.ir_emit();
}