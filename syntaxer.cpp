//
// Created by MUSE on 2018/11/17.
//

#include "syntaxer.h"
#include "lexer.h"
#include "main.h"
#include "error.h"
#include <string.h>
#include <iostream>
#include <vector>
#define BACK back(-- symnumber);
#define MAXTABLEN 512
#define NAMELEN 50

using namespace std;

tab stab;
int level = 0, value, address, number, idx, funcidx = 0;
int symnumber = 0;
int NUM;
bool isbackfromvardec = false;
bool isarray = false;
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

void Syntaxer::enter(char *name, obj object, typ type, int level, int value, int address, int number) {
    if (stab.top > MAXTABLEN){
        error();
        return;
    }
    pushtab(name, object, type, level, value, address, number);
}

void Syntaxer::pushtab(char *name, obj object, typ type, int level, int value, int address, int number){
    if (object == function){
        for (int i = 1; i < stab.pnum; i ++){ // to match the sub program's name
            if (!strcmp(stab.element[stab.pindex[i]].name, name)){
                error(); // TODO
                return;
            }
        }
        stab.pindex[stab.pnum ++] = stab.top + 1; // set this function to the stack's top
    }
    else { // object is const, var, typel
        int i = stab.pindex[stab.pnum - 1]; // find the current sub program
        for ( ; i < stab.top; i ++){
            if (!strcmp(stab.element[i].name, name)){ // if it has been defined
                error(); // TODO
                return;
            }
        }
    }
    strcpy(stab.element[stab.top].name, name);
    stab.element[stab.top].object = object;
    stab.element[stab.top].type = type;
    stab.element[stab.top].level = level;
    stab.element[stab.top].value = value;
    stab.element[stab.top].address = address;
    stab.element[stab.top].number = number;
    stab.top ++;
}

int Syntaxer::searchtab(char *name, obj object) {
    if (object == function) {
        int i;
        for (i = 1; i < stab.pnum; i++) {
            if (!strcmp(stab.element[stab.pindex[i] - 1].name, name)
                && stab.element[stab.pindex[i] - 1].object == function)
                break;
        }
        if (i >= stab.pnum) { // TODO: "==" also ok
            return 0; // hasn't been defined
        }
        return i;
    }
    else { // is not function
        // int i = stab.pindex[stab.pnum - 1];
        int i = 1; // cuz we should visit the external var, for example: when we call a function in the main function.
        for (; i < stab.top; i ++){
            if (!strcmp(stab.element[i].name, name)){
                break;
            }
        }
        if (i == stab.top) {
            return 0;
        }
//        if (i == stab.top){ // hasn't not been found, may be define in the global
//            int len = stab.pindex[1];
//            for (i = 0; i < len; i ++){
//                if (!strcmp(stab.element[i].name, name) && stab.element[i].object == object)
//                    break;
//            }
//            if (i == len){ // undefined
//                return 0;
//            }
//            if (stab.element[i].object == variable ||
//                stab.element[i].object == arrays) {
//                factortype = stab.element[i].type;
//                factorobj = stab.element[i].object;
//                // TODO: should i distinguish array to a boolean?
//                // return stab.element[i].address;
//                return i;
//            }
//        }
        else {
            if (stab.element[i].object == variable ||
                stab.element[i].object == arrays){
                factortype = stab.element[i].type;
                factorobj = stab.element[i].object;
                // TODO: should i distinguish array to a boolean? maybe we should do something here.
                // return stab.element[i].address; // where we can get the value
                return i;
            }
        }
        return i;
    }
}

// --------------------- syntax analysis -------------------
// ＜有返回值函数定义＞ ::= ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
void Syntaxer::retfuncdec() {
    if (symtype == INTSY || symtype == CHARSY) {
        typ typl = symtype == INTSY ? ints : chars;
        nxtsym(); // TODO: enter in tab
        strcpy(symname, token); // function name
        idx = searchtab(symname, function);
        if (idx == 0) {
            enter(symname, function, typl, level + 1, 0, address=0, number);
        }
        else {
            error();
            return;
        }
        funcidx = stab.top - 1;
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        parameterlist(); // get para list length
        stab.element[funcidx].number = number; // attention on this funcidx
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
        stab.top = stab.pindex[stab.pnum - 1];
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
        strcpy(symname, token);
        idx = searchtab(symname, function);
        if (idx == 0) {
            enter(symname, function, voids, level + 1, value=0, address=0, number);
        }
        else {
            error();
            return;
        }
        funcidx = stab.top - 1;
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        parameterlist();
        stab.element[funcidx].number = number;
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
        stab.top = stab.pindex[stab.pnum - 1];
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
    cout << "This is the head of function main." << endl;
    level = 1;
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
        typ typl = symtype == INTSY ? ints : chars;
        nxtsym();
        strcpy(symname, token);
        // TODO : search tab and insert tab.
        idx = searchtab(symname, variable);
        if (idx == 0) {
            address ++;
            enter(symname, variable, typl, level, 0, address, 1);
        }
        else {
            error();
            return;
        }
    }
    nxtsym();
    cout << "This is a parameter." << endl;
}

// ＜参数表＞ ::= ＜参数＞{,＜参数＞}| ＜空>
void Syntaxer::parameterlist() {
    number = 0;
    cout << "This is the head of parameterlist." << endl;
    if (symtype == INTSY || symtype == CHARSY) {
        param();
        number ++;
        if (symtype == COMMASY) {
            do {
                nxtsym();
                param();
                number ++;
            } while(symtype == COMMASY);
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
    number = 0;
    if (symtype == RPARSY) {
        return;
    }
    expression();
    number ++;
    if (symtype == COMMASY) {
        do {
            nxtsym();
            expression();
            number ++;
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
    if (token[0] == '\"') {
        int tokenpoint = 0;
        do {
            tokenpoint ++;
        } while(token[tokenpoint] != '\"');
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a character list." << endl;
}

// ＜字符＞ ::= '＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母＞'｜'＜数字＞'
void Syntaxer::ischaracter() {
    if (token[0] == '\''/*symtype == SQUOSY*/) { // "'"
        // TODO: maybe we should optimize lexer
        value = token[1];
        if (token[2] == '\''/* symtype == SQUOSY */) {
            address++;
            // TODO: we will generate the 4-quad here in the future
        }
        else {
            error();
            return;
        }
    }
}

// ＜整数＞ ::= ［＋｜－］＜无符号整数＞
void Syntaxer::isnumber() {
    if (symtype == PLUSSY || symtype == MINUSSY) { // "+" | "-"
        nxtsym();
        value = transNum(token); // may i rename the "returnname" to " returntoken"?
        value = symtype == PLUSSY ? value : -value;
        address++; // TODO: i don't really understand the address and the num
        // enter(symname, constant, ints, level, value, address, number);
        // TODO: we will generate the 4-quad here in the future
    } else if (symtype == INTEGERSY) {
        //TODO: i forget to deal the first 0 problem
        value = transNum(returnname());
        address++;
        // TODO: we will generate the 4-quad here in the future
    } else {
        error();
        return;
    }
}

// ＜constant(dec)＞ ::= int＜symbol＞＝＜integer＞{,＜symbol＞＝＜integer＞}|char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void Syntaxer::constantdef() {
    // we must have met the "const" before
    if (symtype == INTSY) {
        do {
            nxtsym();
            if (symtype == IDENTSY) { // symbol
                strcpy(symname, token);
                nxtsym();
                if (symtype == ASSIGNSY) { // "="
                    nxtsym();
                    isnumber();
                    idx = searchtab(symname, constant);
                    if (idx == 0) {
                        number = 1;
                        enter(symname, constant, ints, level, value, address, number);
                    }
                    else {
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
        do {
            nxtsym();
            if (symtype == IDENTSY) {
                strcpy(symname, token);
                nxtsym();
                if (symtype == ASSIGNSY) {
                    nxtsym();
                    ischaracter();
                    idx = searchtab(symname, constant);
                    if (idx == 0) {
                        number = 1;
                        enter(symname, constant, chars, level, value, address, number);
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
//                        do {
//                            nxtsym();
//                        }while(symtype != SEMISY); // skip the error var define
                        return;
                    }
                    number = transNum(token); // the length of the array
                    if (number <= 0){
                        error();
                        return;
                    }
                    nxtsym();
                    if (symtype == RBRACKSY){
                        value = 0;
                        address += number;
                        idx = searchtab(symname, arrays);
                        if (idx == 0) {
                            enter(symname, variable, tmpsym == INTSY ? ints : chars, level, value, address, number);
                        }
                        else {
                            error();
                            return;
                        }
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
                    address ++;
                    number = 1;
                    idx = searchtab(symname, variable);
                    if (idx == 0) {
                        enter(symname, variable, tmpsym == INTSY ? ints : chars, level, value, address, number);
                    }
                    else {
                        error();
                        return;
                    }
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

// ＜变量说明＞ ::= ＜变量定义＞;{＜变量定义＞;} int a; int b, c; char d; int e;
void Syntaxer::vardec() {
    cout << "This is the head of vardec." << endl;
    do {
        symbol tmpsym = symtype;
        nxtsym(); // identifier
        nxtsym(); // ( or =
        if (symtype == LPARSY) {
            BACK
            BACK
            symtype = tmpsym;
            isbackfromvardec = true;
            return;
        }
        BACK
        BACK
        symtype = tmpsym;
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
void Syntaxer::program() {
    // initial
    level = 1;
    stab.pnum = 2;
    stab.top = 1;
    stab.pindex[1] = 1;
    cout << "This is the head of progress." << endl;
    if (symtype == CONSTSY) {
        constdec();
    }
    if(symtype == INTSY || symtype == CHARSY || symtype == VOIDSY) {
        do {
            if (symtype == INTSY || symtype == CHARSY) { // vardec or retfuncdec
                symbol tmpsym = symtype;
                nxtsym(); // identifier
                nxtsym(); // '('
                if (symtype != LPARSY) {
                    BACK
                    BACK
                    symtype = tmpsym;
                    vardec();
                } else if (symtype == LPARSY) {
                    BACK
                    BACK
                    symtype = tmpsym;
                    retfuncdec();
                }
                if (isbackfromvardec) {
                    isbackfromvardec = false;
                    retfuncdec();
                }
            }
            if (symtype == VOIDSY) {
                nxtsym();
                if (symtype == MAINSY) {
                    BACK
                    symtype = VOIDSY;
                    functionmain();
                } else {
                    BACK
                    symtype = VOIDSY;
                    voidfuncdec();
                }
            }
        } while (symtype == INTSY || symtype == CHARSY || symtype == VOIDSY);
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
    if (symtype == IFSY) {
        ifstatement();
        return;
    }
    else if (symtype == LOOPSY) {
        whilestatement();
        return;
    }
    else if (symtype == LPRTSY) { // '{'
        nxtsym();
        statementlist();
        if (symtype != RPRTSY) {
            error();
        }
        nxtsym();
        return;
    }
    else if (symtype == IDENTSY) {
        strcpy(symname, token);
        nxtsym();
        if (symtype == ASSIGNSY || symtype == LBRACKSY) {
            BACK
            symtype = IDENTSY;
            idx = searchtab(symname, variable);
            if (idx > 0) {
                assignment();
            }
            else {
                error();
                nxtsym(); // TODO : should i stop?
                return;
            }
        }
        else {
            BACK
            symtype = IDENTSY;
            idx = searchtab(symname, function);
            funcidx = idx;
            if (idx > 0) {
                typ functyp = stab.element[stab.pindex[idx] - 1].type;
                if (functyp == voids) {
                    callvoidfunc();
                }
                else {
                    callretfunc();
                }
            }
            else {
                error();
                nxtsym(); // TODO : should i stop?
                return;
            }
            // callretfunc(); // TODO: we consider it as returning function.
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
        return;
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

// ＜语句列＞ ::= ｛＜语句＞｝
void Syntaxer::statementlist() {
    if (symtype == IFSY || symtype == LOOPSY || symtype == LPRTSY ||
        symtype == IDENTSY || symtype == SCANFSY || symtype == PRINTSY ||
        symtype == SWITCHSY || symtype == RETSY || symtype == SEMISY) {
        do {
            statement();
        } while (symtype == IFSY || symtype == LOOPSY || symtype == LPRTSY ||
                 symtype == IDENTSY || symtype == SCANFSY || symtype == PRINTSY ||
                 symtype == SWITCHSY || symtype == RETSY || symtype == SEMISY); // TODO
    }
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
                    nxtsym();
                    term();
                }
                else {
                    nxtsym();
                    term();
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
    if (symtype == IDENTSY){
        strcpy(symname, token);
        nxtsym();
        if (symtype == LPARSY) { // function
            idx = searchtab(symname, function);
            funcidx = idx;
            if (idx == 0) {
                error();
                return;
            } else if (idx > 0 && stab.element[stab.pindex[idx] - 1].type == voids) { // find bug and fix
                error();
                return;
            }
            nxtsym();
            valuelist();
            if (symtype != RPARSY) {
                error();
//                do {
//                    nxtsym();
//                } while (symtype != SEMISY);
                return;
            }
            nxtsym();
        }
        else if (symtype == LBRACKSY){ // array
            idx = searchtab(symname, arrays);
            if (idx == 0) {
                error();
                return;
            } else if (idx > 0 && stab.element[idx].type == voids) {
                error();
                return;
            }
            nxtsym();
            expression(); // array[ expression ]
            if (symtype != RBRACKSY){ // missing ']'
                error();
//                do {
//                    nxtsym();
//                }while(symtype != SEMISY); // TODO
                return;
            }
            nxtsym();
        }
        else { // identifier -> variable
            idx = searchtab(symname, variable);
            if (idx == 0) {
                error();
                return;
            }
            else {
                // TODO value
            }
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
        }
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
        isnumber();
        nxtsym();
    }
    else if (symtype == SQUOSY){ // character
        ischaracter();
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
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        valuelist();
        if (number != stab.element[stab.pindex[funcidx] - 1].number) {
            error();
            return;
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
    cout << "This is a call of returning function." << endl;
}

// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Syntaxer::callvoidfunc() {
    if (symtype == IDENTSY) {
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        valuelist();
        if (number != stab.element[stab.pindex[funcidx] - 1].number) {
            error();
            return;
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
        else if (symtype == LBRACKSY){ // is array
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

// ＜复合语句＞ ::=［＜常量说明＞］［＜变量说明＞］＜语句列＞
void Syntaxer::compoundstatement() {
    if (symtype == CONSTSY) {
        constdec();
    }
    if (symtype == INTSY || symtype == CHARSY) {
        vardec();
    }
    statementlist(); // it has had a nxtsym();
}

// ＜条件语句＞ ::=  if '('＜条件＞')'＜语句＞
void Syntaxer::ifstatement() {
    cout << "This is the head of if statement." << endl;
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
    cout << "This is a \"if\" statement." << endl;

}

// ＜条件＞ ::= ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
void Syntaxer::condition() {
    cout << "This is the head of condition." << endl;
    expression();
    if (symtype == LSSY || symtype == LESY || symtype == GTSY ||
        symtype == GESY || symtype == NEQSY || symtype == EQUSY){
        nxtsym();
        expression();
    }
    else {
        error();
        return;
    }
    cout << "This is a condition." << endl;
}

// ＜循环语句＞ ::=  while '('＜条件＞')'＜语句＞
void Syntaxer::whilestatement() {
    cout << "This is the head of while statement." << endl;
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
    if (symtype == CASESY) {
        do {
            casestatment();
        } while (symtype == CASESY);
    }
    cout << "This is the case list." << endl;
}

// ＜情况子语句＞::=  case＜常量＞：＜语句＞
void Syntaxer::casestatment() {
    if (symtype == CASESY) {
        nxtsym();
        if (token[0] == '\'') {
            ischaracter();
        }
        else {
            isnumber();
        }
        nxtsym();
        if (symtype != COLONSY) { // ':'
            error();
            return;
        }
        nxtsym();
        statement(); // it has had a nxtsym();
    }
    else {
        error();
        return;
    }
    cout << "This is a case sub statement." << endl;
}

// ＜缺省＞ ::=  default : ＜语句＞|＜空＞
void Syntaxer::defaultstatemnt() {
    cout << "This is the head of default statement." << endl;
    if (symtype == DEFAULTSY) {
        nxtsym();
        if (symtype != COLONSY) {
            error();
            return;
        }
        nxtsym();
        if (symtype != RPRTSY) {
            statement();
        }
        else {
            error();
            return;
        }
    }
    else if (symtype != RPRTSY){
        error();
        return;
    }
    cout << "This is a default statement." << endl;
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
        if (token[0] != '\"') { // isn't string
            expression();
        }
        else if (token[0] == '\"') { // is string
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
            nxtsym();
        }
    }
    else {
        error();
        return;
    }
    cout << "This is a return statement." << endl;
}
