//
// Created by MUSE on 2018/11/17.
//

#include "syntaxer.h"
#include "lexer.h"
#include "main.h"
#include "error.h"
#include <string.h>
#include <iostream>
#define BACK back(-- symnumber);
#define MAXTABLEN 512
#define NAMELEN 50

using namespace std;

tab stab;
int lev = 0, value, addr, number;
int symnumber = 0;
int NUM;
symbol symtype;
typ factortype;
obj factorobj;
char symname[NAMELEN];

void Syntaxer::nxtsym() {
    if (getsym() < 0) exit(0) ;
    symtype = result;
    symnumber ++;
    cout << token << endl;
}

void Syntaxer::enter(char *name, obj object, typ type, int lev, int value, int addr, int num) {
    if (stab.index > MAXTABLEN){
        error();
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
        if (stab.element[stab.pindex[i]].num != number) { // parameter number should be same
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
        nxtsym(); // TODO: enter in tab
        // dechead();
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        parameterlist();
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
    cout << "This is the head of void function" << endl;
    if (symtype == VOIDSY) {
        nxtsym();
        // TODO: enter
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
    cout << "This is the head of parameter." << endl;
    if (symtype == INTSY || symtype == CHARSY){
        nxtsym();
        // TODO : search tab
    }
    nxtsym();
    cout << "This is a parameter." << endl;
}

// ＜参数表＞ ::= ＜参数＞{,＜参数＞}| ＜空>
void Syntaxer::parameterlist() {
    cout << "This is the head of parameterlist." << endl;
    if (symtype == INTSY || symtype == CHARSY) {
        param();
        if (symtype == COMMASY) {
            nxtsym();
            param();
        }
    }
    else if (symtype != RPARSY) {
        error();
        return;
    }
    nxtsym();
    cout << "This is a parameterlist." << endl;
}

// ＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void Syntaxer::valuelist() {
    if (symtype == RPARSY) {
        nxtsym();
        return;
    }
    nxtsym();
    expression();
    if (symtype == COMMASY) {
        do {
            nxtsym();
            expression();
        } while (symtype == COMMASY);
    }
    else if (symtype != RPARSY){
        error();
        return;
    }
    cout << "This is a value list." << endl;
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
    cout << "This is a character list." << endl;
}

// ＜constant(dec)＞ ::= int＜symbol＞＝＜integer＞{,＜symbol＞＝＜integer＞}|char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void Syntaxer::constantdef() {
    // we must have met the "const" before
    if (symtype == INTSY) {
        nxtsym();
        do {
            if (symtype == IDENTSY) { // symbol
                strcpy(symname, token);
                nxtsym();
                if (symtype == ASSIGNSY) { // "="
                    nxtsym();
                    if (symtype == PLUSSY || symtype == MINUSSY) { // "+" | "-"
                        nxtsym();
                        value = transNum(token); // may i rename the "returnname" to " returntoken"?
                        value = symtype == PLUSSY ? value : -value;
                        addr++; // TODO: i don't really understand the address and the num
                        number = -1;
                        enter(symname, constant, ints, lev, value, addr, number);

                        // TODO: we will generate the 4-quad here in the future
                    } else if (symtype == INTEGERSY) {
                        //TODO: i forget to deal the first 0 problem
                        value = transNum(returnname());
                        addr++;
                        number = -1;
                        enter(symname, constant, ints, lev, value, addr, number);
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
            nxtsym();
        }while(symtype == COMMASY);
    }
    else if (symtype == CHARSY){
        nxtsym();
        do {
            if (symtype == IDENTSY) {
                nxtsym();
                if (symtype == ASSIGNSY) {
                    nxtsym();
                    if (token[0] == '\''/*symtype == SQUOSY*/) { // "'"
                        // TODO: maybe we should optimize lexer
                        // nxtsym(); // is a character
                        value = token[1];
                        // nxtsym();
                        if (token[2] == '\''/* symtype == SQUOSY */) {
                            addr++;
                            number = 1;
                            enter(symname, constant, chars, lev, value, addr, number);
                            // TODO: we will generate the 4-quad here in the future
                        }
                        else {
                            error();
                            return;
                        }
                    }
                }
            }
            else {
                error();
                return;
            }
            nxtsym();
        }while(symtype == COMMASY);
    }
    else {
        error();
        return;
    }
    cout << "This is a constant define." << endl;
}

// ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
void Syntaxer::constdec() {
    cout << "This the head of constdec" << endl;
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
        nxtsym();
        if (symtype == CONSTSY) {
            constdec();
        }
    }
    cout << "This is a const declaration." << endl;
}

// ＜声明头部＞ ::= int＜标识符＞ |char＜标识符＞
void Syntaxer::dechead() {
    if (symtype == INTSY) {
        nxtsym(); // identifier
        // TODO : should i search the tab
    }
    else if (symtype == CHARSY){
        nxtsym(); // identifier
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
    cout << "This is the head of vardef." << endl;
    if (symtype == INTSY || symtype == CHARSY){
        symbol tmpsym = symtype;
        number = 0;
        do{
            nxtsym();
            if (symtype == IDENTSY){
                strcpy(symname, token);
                // TODO : enter into tab
                nxtsym();
                if (symtype == LBRACKSY){ // "["
                    nxtsym();
                    if (symtype != INTEGERSY){
                        error();
                        do {
                            nxtsym();
                        }while(symtype != SEMISY); // skip the error var define
                        return;
                    }
                    number = transNum(token);
                    if (number <= 0){
                        error();
                        return;
                    }
                    nxtsym();
                    if (symtype == RBRACKSY){
                        value = number;
                        addr += number;
                        enter(symname, variable, tmpsym == INTSY ? ints : chars, lev, value, addr, number);
                        // nxtsym();
                        // TODO
                    }
                    else {
                        error();
//                        do {
//                            nxtsym();
//                        }while(symtype != SEMISY);
                        return;
                    }
                    nxtsym();
                }
                else {
                    value = 0;
                    addr ++;
                    number = -1;
                    enter(symname, variable, tmpsym == INTSY ? ints : chars, lev, value, addr, number);
                    nxtsym();
                    // TODO
                }
            }
            else {
                error();
//                do {
//                    nxtsym();
//                }while(symtype == COMMASY);
                return;
            }
        }while (symtype == COMMASY);
    }
    cout << "This is a var define." << endl;
}

// ＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;}
void Syntaxer::vardec() {
    cout << "This is the head of vardec." << endl;
    do {
        vardef();
        if (symtype != SEMISY){
            error();
//            do {
//                nxtsym();
//            }while(symtype != INTSY && symtype != CHARSY);
            return;
        }
        nxtsym();
    }while(symtype == INTSY || symtype == CHARSY);
    cout << "This is a var declaration." << endl;
}

// ＜程序＞ ::=［＜常量说明＞］［＜变量说明＞］{＜有返回值函数定义＞|＜无返回值函数定义＞}＜主函数＞
void Syntaxer::progress() {
    if (symtype == CONSTSY) {
        constdec();
    }
    if (symtype == INTSY || symtype == CHARSY) { // vardec or retfuncdec
        symbol tmpsym = symtype;
        nxtsym(); // identifier
        nxtsym(); // '('
        if (symtype == LPARSY) {
            BACK
            BACK
            symtype = tmpsym;
            retfuncdec();
        }
        else {
            BACK
            BACK
            symtype = tmpsym;
            vardec();
        }
    }
    if (symtype == VOIDSY) {
        nxtsym();
        if (symtype == MAINSY) {
            BACK
            symtype = VOIDSY;
            functionmain();
        }
        else {
            BACK
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
        // TODO: jest for logic test
        nxtsym();
        if (symtype == ASSIGNSY) {
            BACK
            symtype = IDENTSY;
            assignment();
        }
        else {
            BACK
            symtype = IDENTSY;
            callretfunc(); // TODO: we consider it as returning function.
        }
//        i = searchtab(returnname(), function);
//        if (i > 0) { // it is the function
//            typ functype = stab.element[stab.pindex[i]].type;
//            if (functype == voids) {
//                nxtsym();
//                callvoidfunc();
//            }
//            else {
//                nxtsym();
//                callretfunc();
//            }
//        }
//        else {
//            i = searchtab(returnname());
//            if (i > 0) {
//                assignment();
//            }
//            else {
//                error();
//                return;
//            }
//        }
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
    cout << "This is the head of expression." << endl;
    if (symtype == PLUSSY || symtype == MINUSSY){
        if (symtype == PLUSSY){
            nxtsym();
            term();
        }
        else {
            nxtsym();
            term();
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
        if (symtype == PLUSSY || symtype == MINUSSY) {
            do {
                if (symtype == PLUSSY || symtype == MINUSSY) {
                    if (symtype == PLUSSY) {
                        nxtsym();
                        term();
                    } else {
                        nxtsym();
                        term();
                        // TODO
                    }
                }
            } while (symtype == PLUSSY || symtype == MINUSSY);
        }
    }
    cout << "This is a expression." << endl;
}

// ＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void Syntaxer::term() {
    cout << "This is the head of term." << endl;
    factor();
    if (symtype == MULTSY || symtype == DIVSY) {
        do {
            if (symtype == MULTSY || symtype == DIVSY) {
                nxtsym();
                factor();
            }
        } while (symtype == MULTSY || symtype == DIVSY);
    }
    cout << "This is a term." << endl;
}

// ＜因子＞ ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞
void Syntaxer::factor() {
    cout << "This is the head of factor." << endl;
    int i;
    if (symtype == IDENTSY){
        nxtsym();
        if (symtype == LPARSY) { // function
            nxtsym();
            strcpy(symname, token);
            valuelist();
            if (symtype != RPRTSY) {
                error();
//                do {
//                    nxtsym();
//                } while (symtype != SEMISY);
                return;
            }
//            i = searchtab(symname, function);
//            if (i <= 0) {
//                error();
//                return;
//            } else if (i > 0 && stab.element[stab.pindex[i]].type == voids) {
//                error();
//                return;
//            }
            nxtsym();
        }
        else if (symtype == LBRACKSY){ // array
            nxtsym();
            expression(); // array[ expression ]
            if (symtype != RBRACKSY){ // missing ']'
                error();
//                do {
//                    nxtsym();
//                }while(symtype != SEMISY); // TODO
                return;
            }
//            i = searchtab(symname, arrays);
//            if (i <= 0){ // undefined
//                error();
//                return;
//            }
            nxtsym();
        }
//        else { // identifier -> variable
//            i = searchtab(symname);
//            if (factorobj == constant){
//                // TODO: i is the constant value
//            }
//            else if (factorobj == variable){
//                // TODO: i is the address in the running stack
//            }
//            else {
//                error();
//                return;
//            }
//        }
    }
    else if (symtype == LPARSY){ // '(' expression ')'
        nxtsym();
        expression(); // have getsym() at last
        if (symtype != RPARSY){ // ')'
            error();
//            do {
//                getsym();
//                symtype = returnsym();
//            }while(symtype == SEMISY);
            return;
        }
        nxtsym();
    }
    else if (symtype == INTEGERSY || symtype == MINUSSY || symtype == PLUSSY){ // [+/-] <integer>
        if (symtype == MINUSSY || symtype == PLUSSY){
            int op = symtype == MINUSSY ? 1 : -1;
            nxtsym();
            if (symtype != INTEGERSY){
                error();
//                do {
//                    nxtsym();
//                }while(symtype != SEMISY);
                return;
            }
            NUM = transNum(returnname()) * op;
            // nxtsym();
        }
        else {
            NUM = transNum(returnname());
            // nxtsym();
        }
        nxtsym();
    }
    else if (symtype == CHARACTERSY){ // character
        nxtsym();
    }
    else {
        error();
    }
    cout << "This is a factor." << endl;
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
        nxtsym();
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
