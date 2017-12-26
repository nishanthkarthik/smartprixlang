#include "parser.h"
#include <sstream>

Term::Term(string& nm)
    : name(nm)
{
    if (nm.empty()) {
        type = TT_UNDEF;
        return;
    }
    if (nm == "<" or nm == ">" or nm == "=" or nm == "~") {
        type = TT_RELOP;
        return;
    }
    char* err;
    value = (int)strtod(nm.c_str(), &err);
    type = (*err == 0) ? TT_VALUE : TT_VARIABLE;
}

ostream& operator<<(ostream& os, const Term& r)
{
    switch (r.type) {
    case TT_VARIABLE:
        os << "(VAR)" << r.name;
        break;
    case TT_VALUE:
        os << "(VAL)" << r.value;
        break;
    case TT_RELOP:
        os << "(REL)" << r.name;
        break;
    default:
        os << "(UNDEF)" << r.name;
    }
    return os;
}

Termtype Term::get_type()
{
    return type;
}

int Term::get_value()
{
    return value;
}

string Term::get_name()
{
    return name;
}

Instruction::Instruction(string& c, vector<Term>& v)
    : cmd(c)
    , args(v)
{
    transform(cmd.begin(), cmd.end(), cmd.begin(), ::toupper);
}

ostream& operator<<(ostream& os, const Instruction& r)
{
    os << r.cmd << " : ";
    for (const Term& t : r.args)
        os << t << " ";
    return os;
}

vector<Instruction> Parser::parse()
{
    vector<Instruction> insvec;
    string line;
    while (getline(*instr, line)) {
        stringstream ss(line);
        string cmdstr;
        ss >> cmdstr;
        if (cmdstr.empty() or cmdstr.front() == ';')
            continue;
        string term;
        vector<Term> termvec;
        do {
            ss >> term;
            Term t(term);
            if (t.get_type() != TT_UNDEF)
                termvec.push_back(move(t));
        } while (!ss.eof());
        insvec.emplace_back(cmdstr, termvec);
    }
    return (code = insvec);
}

string Instruction::get_cmd() const
{
    return cmd;
}

vector<Term> Instruction::get_args() const
{
    return args;
}

Term Instruction::arg_at(int i) const
{
    return args.at(i);
}

string Instruction::name_at(int i)
{
    return args.at(i).get_name();
}

int Instruction::val_at(int i)
{
    return args.at(i).get_value();
}
