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

int Syntaxer::searchtab(char *name, obj object){
    if (object == function){
        int i;
        for (i = 1; i < stab.pnum; i ++){
            if (!strcmp(stab.element[stab.pindex[i]].name, name))
                break;
        }
        if (i >= stab.pnum){ // TODO: "==" also ok
            return 0;
        }
        if (stab.element[stab.pindex[i]].num != num){ // parameter number should be same
            error();
            return -1;
        }
        return i;
    }
    else { // ident: arr, const, var, typel
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
}

// --------------------- syntax analysis -------------------
void Syntaxer::funcdec() {

}

// ＜参数表＞ ::= ＜参数＞{,＜参数＞}| ＜空>
void Syntaxer::parameterlist() {

}

// ＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void Syntaxer::valuelist() {

}

//
void Syntaxer::characterlist() {

}
// ＜constant(dec)＞ ::= int＜symbol＞＝＜integer＞{,＜symbol＞＝＜integer＞}|char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void Syntaxer::constantdef() {
    // we must have met the "const" before
    getsym(); // TODO: should i read a symbol before i analysis it?
    symtype = returnsym();
    if (symtype == INTSY) {
        getsym();
        symtype = returnsym();
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
        getsym();
        symtype = returnsym();
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
    // getsym();
    if (symtype != CONSTSY){ // the symbol before i came in
        error();
        return;
    }
    else {
        getsym();
        symtype = returnsym();
        constantdef();
        if (symtype != SEMISY){
            error();
            return;
        }
        do {
            constdec();
            getsym();
            symtype = returnsym();
        }while(symtype == CONSTSY);
    }
}

void Syntaxer::typedec() {

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
                getsym();
                symtype = returnsym();
            }while(symtype != INTSY && symtype != CHARSY);
            return;
        }
        getsym();
        symtype = returnsym();
    }while(symtype == INTSY || symtype == CHARSY);
}

//
void Syntaxer::progress() {

}

// ＜语句＞ ::= ＜条件语句＞｜＜循环语句＞| '{'＜语句列＞'}'| ＜有返回值函数调用语句＞; |＜无返回值函数调用语句＞;｜＜赋值语句＞;｜＜读语句＞;｜＜写语句＞;｜＜空＞;|＜情况语句＞｜＜返回语句＞;
//＜赋值语句＞ ::=  ＜标识符＞＝＜表达式＞|＜标识符＞'['＜表达式＞']'=＜表达式＞
void Syntaxer::statement() {

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
        getsym();
        symtype = returnsym();
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
        getsym();
        symtype = returnsym();
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
        getsym();
        symtype = returnsym();
    }
    else {
        error();
    }
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

    }
}

// ＜读语句＞ ::= scanf '('＜标识符＞{,＜标识符＞}')'
void Syntaxer::scanfstatement() {

}

// ＜返回语句＞   ::=  return['('＜表达式＞')']
void Syntaxer::returnstatement() {

}
