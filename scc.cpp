#include "emitter.h"
#include "parser.h"
#include <iostream>

int main()
{
    Parser p(&cin);
    auto ins = p.parse();
    // for (auto e : ins)
    //     cout << e << endl;
    Transpiler t(&cout, ins);
    t.transpile_c();
}