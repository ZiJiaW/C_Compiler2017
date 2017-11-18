#include <iostream>
#include <string>
#include "Symbol.h"
using namespace std;
Symbol::Symbol(SymType type, int lineNum, string name, int numVal)
{
    _name = name;
    _type = type;
    _numVal = numVal;
    _lineNum = lineNum;
}
Symbol::Symbol(){}
int Symbol::lineNum()
{
    return _lineNum;
}
string Symbol::name()
{
    return _name;
}
SymType Symbol::type()
{
    return _type;
}
int Symbol::numVal()
{
    return _numVal;
}
string GetTypeName(SymType t)
{
    switch(t)
    {
        case noone: return "noone";
        case END: return "END";
        case ERR: return "ERR";
        case IDENT: return "IDENT";
        case constch: return "constch";
        case conststr: return "conststr";
        case NUM: return "NUM";
        case PLUS: return "PLUS";
        case MINUS: return "MINUS";
        case TIMES: return "TIMES";
        case DIVIDE: return "DIVIDE";
        case EQ: return "EQ";
        case LEQ: return "LEQ";
        case GEQ: return "GEQ";
        case GRT: return "GRT";
        case LES: return "LES";
        case NEQ: return "NEQ";
        case Lpar: return "Lpar";
        case Rpar: return "Rpar";
        case Lspar: return "Lspar";
        case Rspar: return "Rspar";
        case Lbrac: return "Lbrac";
        case Rbrac: return "Rbrac";
        case QUO: return "QUO";
        case DQUO: return "DQUO";
        case COMMA: return "COMMA";
        case SEMICOLON: return "SEMICOLON";
        case COLON: return "COLON";
        case GIVEN: return "GIVEN";
        case symconst: return "symconst";
        case symint: return "symint";
        case symchar: return "symchar";
        case symvoid: return "symvoid";
        case symmain: return "symmain";
        case symif: return "symif";
        case symelse: return "symelse";
        case symfor: return "symfor";
        case symswitch: return "symswitch";
        case symcase: return "symcase";
        case symdefault: return "symdefault";
        case symprintf: return "symprintf";
        case symscanf: return "symscanf";
        case symreturn: return "symreturn";
        default: return "NO SUCH THING!";
    }
}
