#include <iostream>
#include <cstdio>
#include "lexer.h"
#include "syntaxer.h"
#include "quader.h"
#include "asmer.h"
#include "optimizer.h"
#include "tools.h"
#include "error.h"

using namespace std;

char token[MAXN];
symbol result;
string t[REGISTERNUM];
map<string, int>tmpvar;
map<string, int>localvar;
bool optimize = false;
bool iserror = false;

int main() {
//    string judge;
//    cout << "Do you need optimization? [y/n]:";
//    cin >> judge;
//    optimize = judge == "y";
    string path;
    cin >> path;
    init((char*)path.c_str());
    readcode();
    errorinit();
    quaderinit();
    Syntaxer syntaxer;
    syntaxer.nxtsym();
    syntaxer.program();
    if (!iserror) printquads();
    if (!iserror) quad2asm();
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