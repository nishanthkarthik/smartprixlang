#include "ctranspiler.h"
#include "iremitter.h"
#include "parser.h"
#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char const* argv[])
{

    if (argc < 3) {
        cout << "scc llvm|c file|-" << endl;
        cout << "* llvm ir or c output" << endl;
        cout << "* file or stdin" << endl;
        return 1;
    }

    bool isllvm = (string(argv[1]) == "llvm");
    bool isstdin = (string(argv[2]) == "-");

    vector<Instruction> ins;

    if (isstdin) {
        Parser p(&cin);
        ins = p.parse();
    } else {
        ifstream fin(argv[2], ios::in);
        Parser p(&fin);
        ins = p.parse();
        if (fin)
            fin.close();
    }

    if (isllvm) {
        // convert to llvm ir
        IREmitter e(&cout, ins);
        e.ir_emit();
    } else {
        // convert to c
        Transpiler t(&cout, ins);
        t.transpile_c();
    }
}