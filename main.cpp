#include <iostream>
#include <cstdio>
#include "lexer.h"
#include "syntaxer.h"
#include "quader.h"
#include "asmer.h"

using namespace std;

char token[MAXN];
symbol result;

string t[REGISTERNUM];
map<string, int>tmpvar;
map<string, int>localvar;

int main() {
    string path;
    cin >> path;
    init((char*)path.c_str());
    readcode();
    quaderinit();

//    while(getsym() > 0) {}

    Syntaxer syntaxer;
    syntaxer.nxtsym();
    syntaxer.program();
    printquads();
    quad2asm();
//    testoffset();

//    syntaxer.expression();
//    syntaxer.statement();
//    syntaxer.retfuncdec();
//    syntaxer.whilestatement();
//    syntaxer.assignment();
//    syntaxer.switchstatement();
//    syntaxer.printfstatment();
//    syntaxer.scanfstatement();
//    syntaxer.returnstatement();
//    syntaxer.voidfuncdec();
//    syntaxer.retfuncdec();
//    syntaxer.vardec();
//    syntaxer.statement();
    return 0;
}