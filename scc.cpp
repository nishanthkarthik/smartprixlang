#include "ctranspiler.h"
#include "iremitter.h"
#include "parser.h"
#include <iostream>

using namespace std;

int main()
{
    Parser p(&cin);
    auto ins = p.parse();

    // print parsed code
    // for (auto i : ins)
    // cout << i << endl;

    // convert to c
    // Transpiler t(&cout, ins);
    // t.transpile_c();

    // convert to llvm ir
    IREmitter e(&cout, ins);
    e.ir_emit();
}