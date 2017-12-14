#include <fstream>
#include <cstdlib>
#include "MiddleCode.h"
#include "ErrorHandler.h"
#include "Lexer.h"
#include "Symbol.h"
#include "TableItem.h"
#include "SymbolTable.h"
#include "Parser.h"
#include "MIPSTranslator.h"
#include "Optimizer.h"
using namespace std;
string Itos(int n);
string GetMidString(midInstr mc);
int main()
{
    ofstream lexOut, mdOut, mipsOut, optOut, optMips;
    lexOut.open("LexerResult.txt", ios::out);
    mdOut.open("MiddleCode.txt", ios::out);
    mipsOut.open("MIPS.asm", ios::out);
    optOut.open("Optimized.txt", ios::out);
    optMips.open("OptedMIPS.asm", ios::out);
    cout << "Please input source code file path:" << endl;
    string fileName;
    int toOpt = 0;
    cin >> fileName >> toOpt;
    ErrorHandler eh;
    Lexer lex(fileName, eh);
    MiddleCode mc;
    SymbolTable rt;
    Parser par(lex, eh, mc, rt);
    par.StartParsing();
    MIPSTranslator mtr(mc, rt, par.TempTable, par.constZero);
    Optimizer opt(mc, rt, par.TempTable);
    if(eh.IsSuccessful())
    {
        cout<<"Build Succeeded! Now output the result......"<<endl;
        for(vector<Symbol*>::iterator p = par.tokens.begin(); p != par.tokens.end(); p++)
        {
            lexOut<<"In line:"<<(*p)->lineNum()<<"    "<<GetTypeName((*p)->type())<<": "<<(*p)->name()<<endl;
        }
        cout<<"Lexer Result in file: LexerResult.txt!"<<endl;
        cout<<"Now output the MiddleCode......"<<endl;
        for(vector<midInstr>::iterator p = mc.code.begin(); p != mc.code.end(); p++)
        {
            if(GetMidString(*p)!="")
                mdOut<<GetMidString(*p)<<endl;
        }
        cout<<"Middle Code in file: MiddleCode.txt!"<<endl;
        cout<<"Try to translate......"<<endl;
        mtr.translate();
        for(vector<string>::iterator it = mtr.codes.begin(); it != mtr.codes.end(); it++)
            mipsOut<<*it<<endl;
        cout<<"MIPS code in file: MIPS.asm!"<<endl;

        /*********test optimizer*********************/
        if(toOpt)
        {
            cout<<"Now do optimizing......"<<endl;
            opt.optimize();
            cout<<"Output optimized middle code in file: optimized.txt!"<<endl;
            for(vector<midInstr>::iterator p = mc.code.begin(); p != mc.code.end(); p++)
            {
                if(GetMidString(*p)!="")
                    optOut<<GetMidString(*p)<<endl;
            }
            MIPSTranslator optedmtr(mc, rt, par.TempTable, par.constZero);
            optedmtr.translate();
            for(vector<string>::iterator it = optedmtr.codes.begin(); it != optedmtr.codes.end(); it++)
                optMips<<*it<<endl;
            cout<<"Output optimized code in file: OptedMIPS.asm!"<<endl;
        }
        /********************************************/

    }
    else
    {
        eh.PrintError();
    }
    lexOut.close();
    mdOut.close();
    optMips.close();
    optOut.close();
    mipsOut.close();
    return 0;
}
string Itos(int n)
{
    char buffer[20];
    itoa(n, buffer, 10);
    return string(buffer);
}
string GetMidString(midInstr mc)
{
    switch(mc.op)
    {
        case ADD:
        {

            return mc.dst->name()+" = "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name())+" + "+
            string(mc.src2->type()==CONST_CHAR?"\'"+string(1,char(mc.src2->value()))+"\'":
            mc.src2->type()==CONST_INT?Itos(mc.src2->value()):mc.src2->name());
        }
        case SUB:
        {
            return mc.dst->name()+" = "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name())+" - "+
            string(mc.src2->type()==CONST_CHAR?"\'"+string(1,char(mc.src2->value()))+"\'":
            mc.src2->type()==CONST_INT?Itos(mc.src2->value()):mc.src2->name());
        }
        case MUL:
        {
            return mc.dst->name()+" = "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name())+" * "+
            string(mc.src2->type()==CONST_CHAR?"\'"+string(1,char(mc.src2->value()))+"\'":
            mc.src2->type()==CONST_INT?Itos(mc.src2->value()):mc.src2->name());
        }
        case DIV:
        {
            return mc.dst->name()+" = "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name())+" / "+
            string(mc.src2->type()==CONST_CHAR?"\'"+string(1,char(mc.src2->value()))+"\'":
            mc.src2->type()==CONST_INT?Itos(mc.src2->value()):mc.src2->name());
        }
        case GIV:
        {
            return mc.dst->name()+" = "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name());
        }
        case STR:
        {
            return string("");
        }
        case SETL:
        {
            return string("Label_") + mc.dst->name()+":";
        }
        case FUNC:
        {
            string type = string(mc.dst->type()==VOID_FUNC?"void":mc.dst->type()==INT_FUNC?"int":"char");
            return type+" "+mc.dst->name()+"()";
        }
        case PARA:
        {
            string type = string(mc.dst->type()==INT?"int":"char");
            return string("para ")+type+" "+mc.dst->name();
        }
        case PUSH:
        {
            return string("push ")+string(mc.dst->type()==CONST_CHAR?"\'"+string(1,char(mc.dst->value()))+"\'":
            mc.dst->type()==CONST_INT?Itos(mc.dst->value()):mc.dst->name());
        }
        case CALL:
        {
            return string("call ")+mc.dst->name();
        }
        case JMP:
        {
            return string("JMP Label_")+mc.dst->name();
        }
        case NEG:
        {
            return mc.dst->name()+" = -"+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name());
        }
        case RET:
        {
            if(mc.dst!=NULL)
                return string("ret ")+string(mc.dst->type()==CONST_CHAR?"\'"+string(1,char(mc.dst->value()))+"\'":
                mc.dst->type()==CONST_INT?Itos(mc.dst->value()):mc.dst->name());
            else
                return "ret";
        }
        case GET_RET:
        {
            return mc.dst->name()+" = ret";
        }
        case GIV_ARR:
        {
            return mc.dst->name()+"["+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name())+"] = "+
            string(mc.src2->type()==CONST_CHAR?"\'"+string(1,char(mc.src2->value()))+"\'":
            mc.src2->type()==CONST_INT?Itos(mc.src2->value()):mc.src2->name());
        }
        case ARR_GIV:
        {
            return mc.dst->name()+" = "+mc.src1->name()+"["+
            string(mc.src2->type()==CONST_CHAR?"\'"+string(1,char(mc.src2->value()))+"\'":
            mc.src2->type()==CONST_INT?Itos(mc.src2->value()):mc.src2->name())+"]";
        }
        case BLEZ:
        {
            return string("blez Label_")+mc.dst->name()+", "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name());
        }
        case BEQ:
        {
            return string("beq Label_")+mc.dst->name()+", "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name())+", "+
            string(mc.src2->type()==CONST_CHAR?"\'"+string(1,char(mc.src2->value()))+"\'":
            mc.src2->type()==CONST_INT?Itos(mc.src2->value()):mc.src2->name());
        }
        case BGTZ:
        {
            return string("bgtz Label_")+mc.dst->name()+", "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name());
        }
        case BNE:
        {
            return string("bne Label_")+mc.dst->name()+", "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name())+", "+
            string(mc.src2->type()==CONST_CHAR?"\'"+string(1,char(mc.src2->value()))+"\'":
            mc.src2->type()==CONST_INT?Itos(mc.src2->value()):mc.src2->name());
        }
        case BGEZ:
        {
            return string("bgez Label_")+mc.dst->name()+", "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name());
        }
        case BLTZ:
        {
            return string("bltz Label_")+mc.dst->name()+", "+
            string(mc.src1->type()==CONST_CHAR?"\'"+string(1,char(mc.src1->value()))+"\'":
            mc.src1->type()==CONST_INT?Itos(mc.src1->value()):mc.src1->name());
        }
        case WRITE:
        {
            return string("write ")+mc.dst->name();
        }
        case READ:
        {
            return string("read ")+mc.dst->name();
        }
        case END_FUNC:
        {
            return string("end_func ")+mc.dst->name();
        }
        case END_PROC:
        {
            return string("end_prog ")+mc.dst->name();
        }
        default: return "";
    }
}
