//
// Created by MUSE on 2018/11/17.
//

#include "syntaxer.h"
#include "lexer.h"
#include "compiler.h"
#include "error.h"
#include <string.h>
#include <iostream>
#define MAXTABLEN 512
#define NAMELEN 50

using namespace std;

tab stab;
int lev = 0, value, addr, num;
int NUM;
symbol symtype;
typ factortype;
obj factorobj;
char symname[NAMELEN];

void Syntaxer::nxtsym() {
    getsym();
    symtype = returnsym();
}

void Syntaxer::enter(char *name, obj object, typ type, int lev, int value, int addr, int num) {
    if (stab.index > MAXTABLEN){
        error(); // TODO
        return;
    }
    pushtab(name, object, type, lev, value, addr, num);
}

void Syntaxer::pushtab(char *name, obj object, typ type, int lev, int value, int addr, int num){
    if (object == function){
        for (int i = 1; i < stab.pnum; i ++){
            if (!strcmp(stab.element[stab.pindex[i]].name, name)){
                error(); // TODO
                return;
            }
        }
        stab.pindex[stab.pnum ++] = stab.index; // set this function to the stack's top
    }
    else { // object is const, var, typel
        int i = stab.pindex[stab.pnum - 1]; // find the current program
        for ( ; i < stab.index; i ++){
            if (!strcmp(stab.element[i].name, name)){
                error(); // TODO
                return;
            }
        }
    }
    strcpy(stab.element[stab.index].name, name);
    stab.element[stab.index].object = object;
    stab.element[stab.index].type = type;
    stab.element[stab.index].lev = lev;
    stab.element[stab.index].value = value;
    stab.element[stab.index].address = addr;
    stab.element[stab.index].num = num;
}

int Syntaxer::searchtab(char *name, obj object) {
    if (object == function) {
        int i;
        for (i = 1; i < stab.pnum; i++) {
            if (!strcmp(stab.element[stab.pindex[i]].name, name))
                break;
        }
        if (i >= stab.pnum) { // TODO: "==" also ok
            return 0;
        }
        if (stab.element[stab.pindex[i]].num != num) { // parameter number should be same
            error();
            return -1;
        }
        return i;
    }
    else {
        cout << "Please use searchtab(char *name)." << endl;
        return 0;
    }
}

int Syntaxer::searchtab(char *name) {
    // ident: arr, const, var, typel
    int i = stab.pindex[stab.pnum - 1];
    for (; i < stab.index; i ++){
        if (!strcmp(stab.element[i].name, name)){
            break;
        }
    }
    if (i == stab.index){ // may be define in the global
        int len = stab.pindex[1];
        for (i = 0; i < len; i ++){
            if (!strcmp(stab.element[i].name, name))
                break;
        }
        if (i == len){ // undefined
            return 0;
        }
        if (stab.element[i].object == variable ||
            stab.element[i].object == arrays){
            factortype = stab.element[i].type;
            factorobj = stab.element[i].object;
            // TODO: should i distinguish array to a boolean?
            return stab.element[i].address;
        }
        if (stab.element[i].object == constant){
            return stab.element[i].value;
        }
        if (stab.element[i].object == parameter){
            return -1; // TODO
        }
        if (stab.element[i].object == typel){
            return -1;
        }
    }
    else {
        if (stab.element[i].object == variable ||
            stab.element[i].object == arrays){
            factortype = stab.element[i].type;
            factorobj = stab.element[i].object;
            // TODO: should i distinguish array to a boolean?
            return stab.element[i].address;
        }
        if (stab.element[i].object == constant){
            return stab.element[i].value;
        }
        if (stab.element[i].object == parameter){
            return -1; // TODO
        }
        if (stab.element[i].object == typel){
            return -1;
        }
    }
    return 1;
}

// --------------------- syntax analysis -------------------
// ＜有返回值函数定义＞ ::= ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
void Syntaxer::retfuncdec() {
    if (symtype == INTSY || symtype == CHARSY) {
        nxtsym();
        dechead();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        parameterlist();
        if (symtype != RPARSY) {
            error();
            return;
        }
        nxtsym();
        if (symtype != LPRTSY) {
            error();
            return;
        }
        nxtsym();
        compoundstatement();
        if (symtype != RPRTSY) {
            error();
            return;
        }
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a declaration of function which have returning value." << endl;
}

// ＜无返回值函数定义＞ ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void Syntaxer::voidfuncdec() {
    if (symtype == VOIDSY) {
        nxtsym();
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        parameterlist();
        if (symtype != RPARSY) {
            error();
            return;
        }
        nxtsym();
        if (symtype != LPRTSY) {
            error();
            return;
        }
        nxtsym();
        compoundstatement();
        if (symtype != RPRTSY) {
            error();
            return;
        }
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a declaration of function which don\'t have returning value." << endl;
}

// ＜主函数＞ ::= void main'('')''{'＜复合语句＞'}'
void Syntaxer::functionmain() {
    if (symtype == VOIDSY) {
        nxtsym();
        if (symtype != MAINSY) {
            error();
            return;
        }
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        if (symtype != RPARSY) {
            error();
            return;;
        }
        nxtsym();
        if (symtype != LPRTSY) {
            error();
            return;
        }
        nxtsym();
        compoundstatement();
        if (symtype != RPRTSY) {
            error();
            return;
        }
    }
    else {
        error();
        return;
    }
    cout << "This is a declaration of function main." << endl;
}

// ＜参数＞ ::=  ＜类型标识符＞＜标识符＞
void Syntaxer::param() {
    if (symtype == INTSY || symtype == CHARSY){
        nxtsym();
        // TODO : search tab
    }
    nxtsym();
}

// ＜参数表＞ ::= ＜参数＞{,＜参数＞}| ＜空>
void Syntaxer::parameterlist() {
    if (symtype == INTSY || symtype == CHARSY) {
        param();
        if (symtype == COMMASY) {
            do {
                nxtsym();
                param();
            } while (symtype == COMMASY);
        }
    }
    else if (symtype != RPARSY) {
        error();
        return;
    }
}

// ＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void Syntaxer::valuelist() {
    if (symtype == RPARSY) {
        nxtsym();
        return;
    }
    expression();
    if (symtype == COMMASY) {
        do {
            nxtsym();
            expression();
        } while (symtype == COMMASY);
    }
    else {
        error();
        return;
    }
    nxtsym();
}

// ＜字符串＞ ::="｛十进制编码为32,33,35-126的ASCII字符｝"
void Syntaxer::characterlist() {
    if (symtype == DQUOSY) {
        do {
            nxtsym();
        } while(symtype != DQUOSY);
    }
    else {
        error();
        return;
    }
    nxtsym();
}

// ＜constant(dec)＞ ::= int＜symbol＞＝＜integer＞{,＜symbol＞＝＜integer＞}|char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void Syntaxer::constantdef() {
    // we must have met the "const" before
    if (symtype == INTSY) {
        nxtsym();
        do {
            if (symtype == IDENTSY) { // symbol
                strcpy(symname, returnname());
                getsym();
                symtype = returnsym();
                if (symtype == ASSIGNSY) { // "="
                    getsym();
                    symtype = returnsym();
                    if (symtype == PLUSSY || symtype == MINUSSY) { // "+" | "-"
                        getsym();
                        value = transNum(returnname()); // may i rename the "returnname" to " returntoken"?
                        value = symtype == PLUSSY ? value : -value;
                        addr++; // TODO: i don't really understand the address and the num
                        num = -1;
                        enter(symname, constant, ints, lev, value, addr, num);

                        // TODO: we will generate the 4-quad here in the future
                    } else if (symtype == INTEGERSY) {
                        //TODO: i forget to deal the first 0 problem
                        value = transNum(returnname());
                        addr++;
                        num = -1;
                        enter(symname, constant, ints, lev, value, addr, num);
                        // TODO: we will generate the 4-quad here in the future
                    } else {
                        error();
                        return;
                    }
                } else {
                    error();
                    return;
                }
            }
            else {
                error();
                return;
            }
            getsym();
            symtype = returnsym();
        }while(symtype == COMMASY);
    }
    else if (symtype == CHARSY){
        nxtsym();
        do {
            if (symtype == IDENTSY) {
                getsym();
                symtype = returnsym();
                if (symtype == ASSIGNSY) {
                    if (symtype == SQUOSY) { // "'"
                        // TODO: maybe we should optimize lexer
                        getsym();
                        symtype = returnsym(); // is a character
                        value = returnname()[0];
                        getsym();
                        if (returnsym() == SQUOSY) {
                            addr++;
                            num = 1;
                            enter(symname, constant, chars, lev, value, addr, num);
                            // TODO: we will generate the 4-quad here in the future
                        }
                    }
                }
            }
            else {
                error();
                return;
            }
            getsym();
            symtype = returnsym();
        }while(symtype == COMMASY);
    }
    else {
        error();
        return;
    }
}

// ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
void Syntaxer::constdec() {
    if (symtype != CONSTSY){ // the symbol before i came in
        error();
        return;
    }
    else {
        nxtsym();
        constantdef();
        if (symtype != SEMISY){
            error();
            return;
        }
        do {
            constdec();
            nxtsym();
        }while(symtype == CONSTSY);
    }
}

// ＜声明头部＞ ::= int＜标识符＞ |char＜标识符＞
void Syntaxer::dechead() {
    if (symtype == INTSY) {
        nxtsym();
        // TODO : should i search the tab
    }
    else if (symtype == CHARSY){
        nxtsym();
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is the dechead";
}

// ＜变量定义＞ ::= ＜类型标识符＞(＜标识符＞|＜标识符＞'['＜无符号整数＞']'){,(＜标识符＞|＜标识符＞'['＜无符号整数＞']' )}
void Syntaxer::vardef() {
    if (symtype == INTSY || symtype == CHARSY){
        symbol tmpsym = symtype;
        num = 0;
        do{
            getsym();
            symtype = returnsym();
            if (symtype == IDENTSY){
                strcpy(symname, returnname());
                getsym();
                if (symtype == LBRACKSY){ // "["
                    getsym();
                    symtype = returnsym();
                    if (symtype != INTEGERSY){
                        error();
                        do {
                            getsym();
                            symtype = returnsym();
                        }while(symtype != SEMISY); // skip the error var define
                        return;
                    }
                    num = transNum(returnname());
                    if (num <= 0){
                        error();
                        return;
                    }
                    getsym();
                    symtype = returnsym();
                    if (symtype == RBRACKSY){
                        value = num;
                        addr += num;
                        enter(symname, variable, tmpsym == INTSY ? ints : chars, lev, value, addr, num);
                        // TODO
                    }
                    else {
                        error();
                        do {
                            getsym();
                            symtype = returnsym();
                        }while(symtype != SEMISY);
                        return;
                    }
                    getsym();
                    symtype = returnsym();
                }
                else {
                    value = 0;
                    addr ++;
                    num = -1;
                    enter(symname, variable, tmpsym == INTSY ? ints : chars, lev, value, addr, num);
                    // TODO
                }
            }
            else {
                error();
                do {
                    getsym();
                    symtype = returnsym();
                }while(symtype == COMMASY);
            }
        }while (symtype == COMMASY);
    }
}

// ＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
void Syntaxer::vardec() {
    do {
        vardef();
        if (symtype != SEMISY){
            error();
            do {
                nxtsym();
            }while(symtype != INTSY && symtype != CHARSY);
            return;
        }
        nxtsym();
    }while(symtype == INTSY || symtype == CHARSY);
}

// ＜程序＞ ::=［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void Syntaxer::progress() {
    nxtsym();
    if (symtype == CONSTSY) {
        nxtsym();
        constdec();
    }
    else if (symtype == INTSY || symtype == CHARSY) {
        symbol tmpsym = symtype;
        nxtsym(); // identifier
        nxtsym(); // '('
        if (symtype == LPARSY) {
            back();
            back();
            symtype = tmpsym;
            retfuncdec();
        }
        else {
            back();
            back();
            symtype = tmpsym;
            vardec();
        }
    }
    else if (symtype == VOIDSY) {
        nxtsym();
        if (symtype == MAINSY) {
            back();
            symtype = VOIDSY;
            functionmain();
        }
        else {
            back();
            symtype = VOIDSY;
            voidfuncdec();
        }
    }
    else {
        error();
        return;
    }
    nxtsym();
}

// ＜语句＞ ::= ＜条件语句＞｜＜循环语句＞| '{'＜语句列＞'}'| ＜有返回值函数调用语句＞; |＜无返回值函数调用语句＞;
// ｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;
void Syntaxer::statement() {
    int i;
    if (symtype == IFSY) {
        nxtsym();
        ifstatement();
    }
    else if (symtype == LOOPSY) {
        nxtsym();
        whilestatement();
    }
    else if (symtype == LPRTSY) {
        nxtsym();
        statementlist();
        if (symtype != RPRTSY) {
            error();
            return;
        }
        nxtsym();
    }
    else if (symtype == IDENTSY) {
        i = searchtab(returnname(), function);
        if (i > 0) { // it is the function
            typ functype = stab.element[stab.pindex[i]].type;
            if (functype == voids) {
                nxtsym();
                callvoidfunc();
            }
            else {
                nxtsym();
                callretfunc();
            }
        }
        else {
            i = searchtab(returnname());
            if (i > 0) {
                assignment();
            }
            else {
                error();
                return;
            }
        }
    }
    else if (symtype == SCANFSY) {
        scanfstatement();
    }
    else if (symtype == PRINTSY) {
        printfstatment();
    }
    else if (symtype == SWITCHSY) {
        switchstatement();
    }
    else if (symtype == RETSY) {
        returnstatement();
    }
    else if (symtype != SEMISY) {
        error();
        return;
    }
    if (symtype != SEMISY) {
        error();
        return;;
    }
    nxtsym();
    cout << "This is a statement." << endl;
}

// ＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   //[+|-]只作用于第一个<项>
void Syntaxer::expression() {
    if (symtype == PLUSSY || symtype == MINUSSY){
        if (symtype == PLUSSY){
            getsym();
            symtype = returnsym();
            term();
        }
        else {
            // TODO
        }
        if (symtype == PLUSSY || symtype == MINUSSY) {
            do {
                if (symtype == PLUSSY) {
                    getsym();
                    symtype = returnsym();
                    term();
                }
                else {
                    // TODO
                }
            } while (symtype == PLUSSY || symtype == MINUSSY);
        }
    }
    else {
        term();
        do {
            if (symtype == PLUSSY || symtype == MINUSSY) {
                if (symtype == PLUSSY) {
                    getsym();
                    symtype = returnsym();
                    term();
                } else {
                    // TODO
                }
            }
        } while (symtype == PLUSSY || symtype == MINUSSY);
    }
}

// ＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void Syntaxer::term() {
    factor();
    do {
        if (symtype == MULTSY || symtype == DIVSY){
            getsym();
            symtype = returnsym();
            factor();
        }
    }while(symtype == MULTSY || symtype == DIVSY);
}

// ＜因子＞ ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞
void Syntaxer::factor() {
    int i;
    if (symtype == IDENTSY){
        nxtsym();
        if (symtype == LPARSY) { // function
            getsym();
            symtype = returnsym();
            strcpy(symname, returnname());
            valuelist();
            if (symtype != RPRTSY) {
                error();
                do {
                    getsym();
                    symtype = returnsym();
                } while (symtype != SEMISY);
                return;
            }
            i = searchtab(symname, function);
            if (i <= 0) {
                error();
                return;
            } else if (i > 0 && stab.element[stab.pindex[i]].type == voids) {
                error();
                return;
            }
            getsym();
            symtype = returnsym();
        }
        else if (symtype == LBRACKSY){ // array
            getsym();
            symtype = returnsym();
            expression(); // array[ expression ]

            if (symtype != RBRACKSY){ // missing ']'
                error();
                do {
                    getsym();
                    symtype = returnsym();
                }while(symtype != SEMISY); // TODO
                return;
            }
            i = searchtab(symname, arrays);
            if (i <= 0){ // undefined
                error();
                return;
            }
            getsym();
            symtype = returnsym();
        }
        else { // identifier -> variable
            i = searchtab(symname, variable);
            if (factorobj == constant){
                // TODO: i is the constant value
            }
            else if (factorobj == variable){
                // TODO: i is the address in the running stack
            }
            else {
                error();
                return;
            }
            // getsym();
            // symtype = returnsym();
        }
    }
    else if (symtype == LPARSY){ // '(' expression ')'
        nxtsym();
        expression(); // have getsym() at last
        if (symtype != RPARSY){ // ')'
            error();
            do {
                getsym();
                symtype = returnsym();
            }while(symtype == SEMISY);
            return;
        }
        getsym();
    }
    else if (symtype == INTEGERSY || symtype == MINUSSY || symtype == PLUSSY){ // [+/-] <integer>
        if (symtype == MINUSSY || symtype == PLUSSY){
            int op = symtype == MINUSSY ? 1 : -1;
            getsym();
            symtype = returnsym();
            if (symtype != INTEGERSY){
                error();
                do {
                    getsym();
                    symtype = returnsym();
                }while(symtype != SEMISY);
                return;
            }
            NUM = transNum(returnname()) * op;
            getsym();
            symtype = returnsym();
        }
        else {
            NUM = transNum(returnname());
            getsym();
            symtype = returnsym();
        }
    }
    else if (symtype == CHARACTERSY){ // character
        nxtsym();
    }
    else {
        error();
    }
}

// ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Syntaxer::callretfunc() {
    if (symtype == IDENTSY) {
        nxtsym();
        valuelist();
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a call of returning function." << endl;
}

// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Syntaxer::callvoidfunc() {
    if (symtype == IDENTSY) {
        nxtsym();
        valuelist();
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a call of void function." << endl;
}

// ＜赋值语句＞ ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
// <赋值语句> ::= <标识符>(=<表达式>|'['<表达式>']'=<表达式>)
void Syntaxer::assignment() {
    if (symtype == IDENTSY) {
        getsym();
        returnsym();
        if (symtype == ASSIGNSY) {
            nxtsym();
            expression();
        }
        else if (symtype == LBRACKSY){
            nxtsym();
            expression();
            if (symtype != RBRACKSY){
                error();
                return;
            }
            nxtsym();
            if (symtype == ASSIGNSY){
                nxtsym();
                expression();
            }
        }
    }
    else {
        error();
        return;
    }
    cout << "This is a assignment statement." << endl; // it will break if something in the middle.
}

// ＜语句列＞ ::= ｛＜语句＞｝
void Syntaxer::statementlist() {
    do {
        statement();
    }while (symtype == IFSY); // TODO
}

// ＜复合语句＞ ::=［＜常量说明＞］［＜变量说明＞］＜语句列＞
void Syntaxer::compoundstatement() {
    if (symtype == CONSTSY) {
        constdec();
    }
    else if (symtype == INTSY || symtype == CHARSY) {
        vardec();
    }
    statementlist();
    nxtsym();
}

// ＜条件语句＞ ::=  if '('＜条件＞')'＜语句＞
void Syntaxer::ifstatement() {
    if (symtype == IFSY){
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        condition();
        if (symtype != RPARSY) {
            error();
            return;
        }
        nxtsym();
        statement();
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a \"if\" statement." << endl;

}

// ＜条件＞ ::= ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
void Syntaxer::condition() {
    expression();
    nxtsym();
    if (symtype == LSSY || symtype == LESY || symtype == GTSY ||
        symtype == GESY || symtype == NEQSY || symtype == EQUSY){
        nxtsym();
        expression();
    }
    nxtsym();
    cout << "This is a condition." << endl;
}

// ＜循环语句＞ ::=  while '('＜条件＞')'＜语句＞
void Syntaxer::whilestatement() {
    if (symtype == LOOPSY) {
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        condition();
        if (symtype != RPARSY) {
            error();
            return;
        }
        nxtsym();
        statement();
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a recurrent statement." << endl;
}

// ＜情况语句＞ ::=  switch '('＜表达式＞')' '{'＜情况表＞＜缺省＞ '}'
void Syntaxer::switchstatement() {
    if (symtype == SWITCHSY) {
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        expression();
        if (symtype != RPARSY) {
            error();
            return;
        }
        nxtsym();
        if (symtype != LPRTSY) {
            error();
            return;;
        }
        nxtsym();
        caselist();
        nxtsym();
        defaultstatemnt();
        if (symtype != RPRTSY) {
            error();
            return;
        }
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a switch statement." << endl;
}

// ＜情况表＞ ::= ＜情况子语句＞{＜情况子语句＞}
void Syntaxer::caselist() {
    casestatment();
    nxtsym();
    if (symtype == CASESY) {
        do {
            casestatment();
            nxtsym();
        } while (symtype == CASESY);
    }
    cout << "This is the case list." << endl;
}

// ＜情况子语句＞::=  case＜常量＞：＜语句＞
void Syntaxer::casestatment() {
    if (symtype == CASESY) {
        nxtsym();
        constantdef();
        if (symtype != COLONSY) {
            error();
            return;
        }
        nxtsym();
        statement();
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a case sub statement." << endl;
}

// ＜缺省＞ ::=  default : ＜语句＞|＜空＞
void Syntaxer::defaultstatemnt() {
    if (symtype == DEFAULTSY) {
        nxtsym();
        if (symtype != COLONSY) {
            error();
            return;
        }
        nxtsym();
        if (symtype != RPRTSY) {
            nxtsym();
            statement();
        }
    }
    else {
        error();
        return;
    }
}

// ＜写语句＞ ::= printf ('(' ＜字符串＞(,＜表达式＞|<空> ') | '('＜表达式＞')')
void Syntaxer::printfstatment() {
    if (symtype == PRINTSY) {
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        if (symtype != DQUOSY) {
            nxtsym();
            expression();
        }
        else if (symtype == DQUOSY) {
            nxtsym();
            characterlist();
            if (symtype == COMMASY) {
                nxtsym();
                expression();
            }
        }
        if (symtype != RPARSY) {
            error();
            return;
        }
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a printf statement." << endl;
}

// ＜读语句＞ ::= scanf '('＜标识符＞{,＜标识符＞}')'
void Syntaxer::scanfstatement() {
    if (symtype == SCANFSY) {
        nxtsym();
        if (symtype == LPARSY) {
            nxtsym();
            // TODO : search the tab?
            nxtsym();
            if (symtype == COMMASY) {
                do {
                    nxtsym();
                    nxtsym();
                } while (symtype == COMMASY);
            }
        }
        if (symtype != RPARSY) {
            error();
            return;
        }
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a scanf statement." << endl;
}

// ＜返回语句＞ ::= return['('＜表达式＞')']
void Syntaxer::returnstatement() {
    if (symtype == RETSY) {
        nxtsym();
        if (symtype == LPARSY) {
            nxtsym();
            expression();
            if (symtype != RPARSY) {
                error();
                return;
            }
        }
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a return statement." << endl;
}
