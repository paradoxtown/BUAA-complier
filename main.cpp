#include <iostream>
#include <cstdio>
#include "lexer.h"
#include "syntaxer.h"

using namespace std;

char token[MAXN];
symbol result;

int main() {
    string path;
    cin >> path;
    init((char*)path.c_str());
    readcode();
//    while(getsym() > 0) {}
    Syntaxer syntaxer;
    syntaxer.nxtsym();
//    syntaxer.progress();
//    syntaxer.expression();
//    syntaxer.statement();
//    syntaxer.retfuncdec();
    syntaxer.whilestatement();
//    syntaxer.assignment();
    return 0;
}