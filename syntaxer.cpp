

//
// Created by MUSE on 2018/11/17.
//
#include "syntaxer.h"
#include "lexer.h"
#include "main.h"
#include "error.h"
#include "quader.h"
#include <string.h>
#include <iostream>
#include <vector>
#define BACK back(-- symnumber);
#define MAXTABLEN 512
#define NAMELEN 50

using namespace std;

tab stab;
int level = 0, value, address, number, idx, funcidx = 0;
int regnum = 0, labelnum = 0, strnum = 0;
int ischar = 0;
string labeln, labelm, switchout, tn, tm, to;
string switchcase;
int symnumber = 0;
bool isbackfromvardec = false;
bool havemain = false;
//bool lock = true;
symbol symtype;
char symname[NAMELEN];
char name4back[NAMELEN];

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
                error();
                return;
            }
        }
        stab.pindex[stab.pnum ++] = stab.top + 1; // set this function to the stack's top
    }
    else { // object is const, var, typel
        int i = stab.pindex[stab.pnum - 1]; // find the current sub program
        for ( ; i < stab.top; i ++){
            if (!strcmp(stab.element[i].name, name) && stab.element[i].object == object){ // if it has been defined
                error();
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
        if (i >= stab.pnum) {
            return 0; // hasn't been defined
        }
        return i;
    }
    else { // is not function
        int i = stab.top - 1; // cuz we should visit the external var, for example: when we call a function in the main function.
        for (; i > 0; i --){ // and we should use the latest value to confirm our query object
            if (!strcmp(stab.element[i].name, name) && stab.element[i].object == object){
                break;
            }
        }
        if (i == 0) {
            return 0;
        }
        return i;
    }
}

// --------------------- syntax analysis -------------------
// ＜有返回值函数定义＞ ::= ＜声明头部＞'('＜参数表＞')' '{'＜复合语句＞'}'
void Syntaxer::retfuncdec() {
    if (symtype == INTSY || symtype == CHARSY) {
        address = 0;
        regnum = 0; // TODO: in the function the register can start from 0
        emit(label, symname, "", ""); // TODO: the function name shouldn't be same with the inst
        char funcname[NAMELEN];
        typ typl = symtype == INTSY ? ints : chars;
        nxtsym();
        strcpy(funcname, token); // function name
        idx = searchtab(funcname, function);
        if (idx == 0) {
            enter(funcname, function, typl, level, 0, address, number); // we need to set space for function
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
        stab.element[funcidx].number = number; // attention on this funcidx, reset the number of function
        emit(func, typl == ints ? "int" : "char", funcname, to_string(number)); // we need to use emit2
        if (symtype != LPRTSY) {
            error();
            return;
        }
        nxtsym();
        compoundstatement();
        tmpvar[funcname] = regnum; // record the number of tmpvar
        localvar[funcname] = address / 4;
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
    if (symtype == VOIDSY) {
        address = 0;
        regnum = 0;
        emit(label, symname, "", "");
        char funcname[NAMELEN];
        nxtsym();
        strcpy(funcname, token);
        strcpy(symname, token);
        idx = searchtab(symname, function);
        if (idx == 0) {
            enter(symname, function, voids, level, value=0, address, number);
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
        emit(func, "void", funcname, to_string(number));
        if (symtype != LPRTSY) {
            error();
            return;
        }
        nxtsym();
        compoundstatement();
        tmpvar[funcname] = regnum;
        localvar[funcname] = address / 4;
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
    if (symtype == VOIDSY) {
        address = 0;
        regnum = 0;
        emit(label, "main", "", "");
        emit(func, "void", "main", "0");
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
        tmpvar["main"] = regnum;
        localvar["main"] = address / 4;
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
        idx = searchtab(symname, parameter);
        if (idx == 0) {
            enter(symname, parameter, typl, level, 0, address, 1);
            emit(para, typl == ints ? "int" : "char", symname, "");
            address += 4;
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
        // empty, no emit
        return;
    }
    expression(false);
    number ++;
    int tmpaddr = 0; // be relative to the function's address in the stack, this is the offset
    // tn = "t" + to_string(regnum - 1);
    emit(push, to, "", "", false, tmpaddr); // TODO: use to register which is the returning value from expression
    tmpaddr += 4;
    if (symtype == COMMASY) {
        do {
            nxtsym();
            expression(false);
            number ++;
            // tn = "t" + to_string(regnum - 1);
            emit(push, to, "", "", false, tmpaddr);
            tmpaddr += 4;
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
    cout << "The string is " << token << endl;
    if (token[0] == '\"') {
        int tokenpoint = 1;
        do {
            // emit(write, to_string(int(token[tokenpoint])), "1", "");
            tokenpoint ++;
        } while(token[tokenpoint] != '\"');
        emit(str, token, "_str" + to_string(strnum),  "", true, 0);
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
        value = token[1];
        if (token[2] == '\''/* symtype == SQUOSY */) {
            address++;
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
    } else if (symtype == INTEGERSY) {
        value = transNum(returnname());
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
                        emit(cons, "int", symname, to_string(value), level == 1, address);
                        address += 4;
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
                        address += 4;
                        emit(cons, "char", symname, to_string(value), level == 1, address); // token is 'ch'
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
                nxtsym();
                if (symtype == LBRACKSY){ // "[" ==> array
                    nxtsym();
                    if (symtype != INTEGERSY){
                        error();
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
                        idx = searchtab(symname, variable);
                        if (idx == 0) {
                            enter(symname, variable, tmpsym == INTSY ? ints : chars, level, value, address, number);
                            emit(arr, tmpsym == INTSY ? "int" : "char", symname, to_string(number), level == 1, address); // array
                            address += number * 4;
                        }
                        else {
                            error();
                            return;
                        }
                    }
                    else {
                        error();
                        return;
                    }
                    nxtsym();
                }
                else { // is not array
                    value = 0;
                    number = 1;
                    idx = searchtab(symname, variable);
                    if (idx == 0) {
                        enter(symname, variable, tmpsym == INTSY ? ints : chars, level, value, address, number);
                        emit(var, tmpsym == INTSY ? "int" : "char", symname, "", level == 1, address);
                        address += 4;
                    }
                    else {
                        error();
                        return;
                    }
                }
            }
            else {
                error();
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
        strcpy(symname, token);
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
    address = 0;
    bool funcdecbegin = true;
    cout << "This is the head of progress." << endl;
    if (symtype == CONSTSY) {
        constdec();
    }
    if(symtype == INTSY || symtype == CHARSY || symtype == VOIDSY) {
        do {
            if (symtype == INTSY || symtype == CHARSY) { // vardec or retfuncdec
                symbol tmpsym = symtype;
                nxtsym(); // identifier
                strcpy(symname, token);
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
                    if (funcdecbegin) {
                        emit(jump, "main", "", "");
                        funcdecbegin = false;
                        level = 2; // use for judge whether is global
                    }
                    retfuncdec();
                }
                if (isbackfromvardec) {
                    emit(jump, "main", "", "");
                    funcdecbegin = false;
                    level = 2; // use for judge whether is global
                    isbackfromvardec = false;
                    retfuncdec();
                }
            }
            if (symtype == VOIDSY) {
                level = 2;
                nxtsym();
                strcpy(symname, token);
                if (symtype == MAINSY) {
                    BACK
                    havemain = true;
                    symtype = VOIDSY;
                    functionmain();
                } else {
                    BACK
                    symtype = VOIDSY;
                    if (funcdecbegin) {
                        emit(jump, "main", "", "");
                        funcdecbegin = false;
                    }
                    voidfuncdec();
                }
            }
        } while (symtype == INTSY || symtype == CHARSY || symtype == VOIDSY);
        if (!havemain) {
            error();
            return;
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
        strcpy(name4back, token);
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
            string tmpname = symname;
            symtype = IDENTSY;
            idx = searchtab(symname, function);
            funcidx = idx;
            if (idx > 0) {
                typ functyp = stab.element[stab.pindex[idx] - 1].type;
                if (functyp == voids) {
                    callvoidfunc(tmpname);
                }
                else {
                    callretfunc(tmpname);
                }
            }
            else {
                error();
                nxtsym(); // TODO : should i stop?
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
                 symtype == SWITCHSY || symtype == RETSY || symtype == SEMISY);
    }
}

// ＜表达式＞ ::= ［＋｜－］＜项＞{＜加法运算符＞＜项＞}   //[+|-]只作用于第一个<项>
void Syntaxer::expression(bool lock) {
    cout << "This is the head of expression." << endl;
    if (symtype == PLUSSY || symtype == MINUSSY){
        if (symtype == PLUSSY){ // + a + b, there is no influence
            nxtsym();
            term(lock);
        }
        else { // - a + b
            nxtsym();
            term(lock);
            string preto = to;
            to = "t" + to_string(regnum);
            regnum ++;
            emit(sub, to, "0", preto); // to = 0 - tn
        }
        string pretn = to;
        if (symtype == PLUSSY || symtype == MINUSSY) {
            do {
                symbol tmpsym = symtype;
                pretn = to;
                nxtsym();
                term(lock);
                string preto = to;
                to = "t" + to_string(regnum);
                regnum ++;
                emit(tmpsym == PLUSSY ? add : sub, to, pretn, preto); // to = tn op tm
            } while (symtype == PLUSSY || symtype == MINUSSY);
        }
    }
    else { // a + b
        term(lock);
        string pretn = to;
        if (symtype == PLUSSY || symtype == MINUSSY) {
            do {
                symbol tmpsym = symtype;
                pretn = to;
                nxtsym();
                term(lock);
                string preto = to;
                to = "t" + to_string(regnum);
                regnum ++;
                emit(tmpsym == PLUSSY ? add : sub, to, pretn, preto); // to = tn op tm
            } while (symtype == PLUSSY || symtype == MINUSSY);
        }
    }
    cout << "This is a expression." << endl;
}

// ＜项＞ ::= ＜因子＞{＜乘法运算符＞＜因子＞}
void Syntaxer::term(bool lock) {
    cout << "This is the head of term." << endl;
    factor(lock);
    string pretn = to;
    if (symtype == MULTSY || symtype == DIVSY) {
        do {
            symbol tmpsym = symtype;
            pretn = to; // current to is the tn = "t" + to_string(regnum - 1); but we need the value if it is constant
            nxtsym();
            factor(lock);
            string preto = to;
            to = "t" + to_string(regnum);
            regnum ++;
            emit(tmpsym == MULTSY ? mult : divide, to, pretn, preto); // to = tn op tm
        } while (symtype == MULTSY || symtype == DIVSY);
    }
    cout << "This is a term." << endl;
}

// ＜因子＞ ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞
void Syntaxer::factor(bool lock) {
    cout << "This is the head of factor." << endl;
    to = "t" + to_string(regnum);
    string tmpto = to;
    if (symtype == IDENTSY){
        char tmpname[NAMELEN];
        strcpy(tmpname, token);
        strcpy(symname, token);
        nxtsym();
        if (symtype == LPARSY) { // function
            regnum ++;
            idx = searchtab(symname, function);
            funcidx = idx;
            if (idx == 0) {
                error();
                return;
            } else if (idx > 0 && stab.element[stab.pindex[idx] - 1].type == voids) { // find bug and fix
                error();
                return;
            }
            emit(call, tmpname, "", "");
            if(lock) stab.element[stab.pindex[idx] - 1].type == ints ? ischar += 2 : ischar ++;
            nxtsym();
            valuelist();
            if (symtype != RPARSY) {
                error();
                return;
            }
            emit(jal, tmpname, "", "");
            emit(fact, tmpto, "$v0", "");
            to = tmpto;
            nxtsym();
        }
        else if (symtype == LBRACKSY){ // array
            regnum ++;
            idx = searchtab(symname, variable);
            if (idx == 0) {
                error();
                return;
            } else if (idx > 0 && stab.element[idx].type == voids) {
                error();
                return;
            }
            if(lock) stab.element[idx].type == ints ? ischar += 2 : ischar ++;
            int tmpaddr = stab.element[idx].address;
            bool isglobal = stab.element[idx].level == 1;
            nxtsym();
//            lock = false;
            expression(false); // array[ expression ]
//            lock = true;
            if (symtype != RBRACKSY){ // missing ']'
                error();
                return;
            }
            emit(fact, tmpto, tmpname, to, isglobal, tmpaddr); // tn is the expression's value, fact to arr, i
            to = tmpto;
            nxtsym();
        }
        else { // identifier -> variable
            regnum ++;
            idx = searchtab(symname, parameter);
            if (idx == 0) {
                idx = searchtab(symname, variable) + searchtab(symname, constant);
                if (idx == 0) {
                    error();
                    return;
                }
            }
            if(lock) stab.element[idx].type == ints ? ischar += 2 : ischar ++;
            int tmpaddr = stab.element[idx].address;
            bool isglobal = stab.element[idx].level == 1;
            emit(fact, tmpto, tmpname, "", isglobal, tmpaddr); // fact to a
            to = tmpto;
            // why there is no nxtsym, cuz we have done it when we get in
        }
    }
    else if (symtype == LPARSY){ // '(' expression ')'
        nxtsym();
        expression(lock); // have getsym() at last
        if (symtype != RPARSY){ // ')'
            error();
            return;
        }
        if (lock) ischar += 2;
        nxtsym();
    }
    else if (symtype == INTEGERSY || symtype == MINUSSY || symtype == PLUSSY){ // [+/-] <integer>
        isnumber();
        if(lock) ischar += 2;
        to = to_string(value);
        nxtsym();
    }
    else if (symtype == SQUOSY){ // character
        ischaracter();
        if(lock) ischar ++;
        to = to_string(value);
        nxtsym();
    }
    else {
        error();
        return;
    }
    cout << "This is a factor." << endl;
}

// ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Syntaxer::callretfunc(string funcname) {
    if (symtype == IDENTSY) {
        emit(call, funcname, "", "");
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
        emit(jal, funcname, "", "");
    }
    else {
        error();
        return;
    }
    nxtsym();
    cout << "This is a call of returning function." << endl;
}

// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Syntaxer::callvoidfunc(string funcname) {
    if (symtype == IDENTSY) {
        emit(call, funcname, "", "");
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
        emit(jal, funcname, "", "");
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
            expression(false);

            idx = searchtab(name4back, parameter);
            if (idx == 0) {
                idx = searchtab(name4back, variable);
                if (idx == 0) {
                    error();
                    return;
                }
            }
            int tmpaddr = stab.element[idx].address;
            bool isglobal = stab.element[idx].level == 1;
            emit(assign, name4back, to, "", isglobal, tmpaddr); // assign a tn
        }
        else if (symtype == LBRACKSY){ // is array '['
            nxtsym();
            expression(false);
            // string tmptn = "t" + to_string(regnum - 1); // the length of array
            string preto = to;
            if (symtype != RBRACKSY){ //
                error();
                return;
            }
            nxtsym();
            if (symtype == ASSIGNSY){
                nxtsym();
                expression(false);

                idx = searchtab(name4back, variable);
                if (idx == 0) {
                    error();
                    return;
                }
                int tmpaddr = stab.element[idx].address;
                bool isglobal = stab.element[idx].level == 1;
                emit(assign, name4back, preto, to, isglobal, tmpaddr); // assign a i b
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
        labeln = "label" + to_string(labelnum);
        labelnum ++;
        emit(jump, labeln, "", "");
        emit(label, labelm, "", "");
        string tmplabel = labeln;
        nxtsym();
        statement();
        // if statement also has label, so we should use the latest labelnum
        emit(label, tmplabel, "", "");
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
    expression(false);
    string preto = to;
    if (symtype == LSSY || symtype == LESY || symtype == GTSY ||
        symtype == GESY || symtype == NEQSY || symtype == EQUSY){
        symbol tmpsym = symtype;
        nxtsym();
        expression(false);
        labelm = "label" + to_string(labelnum);
        labelnum ++;

        switch(tmpsym){
            case LSSY:  emit(bltz, preto, to, labelm); break;
            case LESY:  emit(blez, preto, to, labelm); break;
            case GTSY:  emit(bgtz, preto, to, labelm); break;
            case GESY:  emit(bgez, preto, to, labelm); break;
            case EQUSY: emit(beq, preto, to, labelm); break;
            case NEQSY: emit(bne, preto, to, labelm); break;
            default:;
        }
    }
    else {
        error();
        return;
    }
    cout << "This is a condition." << endl;
}

// ＜循环语句＞ ::=  while '('＜条件＞')'＜语句＞
void Syntaxer::whilestatement() {
    string whileout = "label" + to_string(labelnum);
    labelnum ++;
    string beginlabel = "label" + to_string(labelnum);
    labelnum ++;
    emit(label, beginlabel, "", "");

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

        emit(jump, whileout, "", ""); // if not equal
        emit(label, labelm, "", "");

        nxtsym();
        statement();

        emit(jump, beginlabel, "", "");
    }
    else {
        error();
        return;
    }
    emit(label, whileout, "", "");
    cout << "This is a recurrent statement." << endl;
}

// ＜情况语句＞ ::=  switch '('＜表达式＞')' '{'＜情况表＞＜缺省＞ '}'
void Syntaxer::switchstatement() {
    switchout = "label" + to_string(labelnum);
    labelnum ++;
    string tmpswitchout = switchout;
    if (symtype == SWITCHSY) {
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        expression(false);
        switchcase = to;
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
    emit(label, tmpswitchout, "", "");
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
            ischaracter(); // we don't use register in it, so we needn't to use tmp
        }
        else {
            isnumber();
        }

        nxtsym();
        if (symtype != COLONSY) { // ':'
            error();
            return;
        }
        string tmpswitchcase = switchcase;
        labelm = "label" + to_string(labelnum);
        labelnum ++;
        labeln = "label" + to_string(labelnum);
        labelnum ++;
        emit(beq, tmpswitchcase, to_string(value), labelm);
        emit(jump, labeln, "", ""); // if not equal
        emit(label, labelm, "", "");
        string tmplabel = labeln;
        string tmpswitchout = switchout;
        nxtsym();
        statement();

        emit(jump, tmpswitchout, "", "");
        emit(label, tmplabel, "", "");
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
    ischar = 0;
    if (symtype == PRINTSY) {
        nxtsym();
        if (symtype != LPARSY) {
            error();
            return;
        }
        nxtsym();
        if (token[0] != '\"') { // isn't string
            expression(true);
            emit(write, to, to_string(ischar), "");
        }
        else if (token[0] == '\"') { // is string
            characterlist(); // the emit is in it
            emit(write, "_str" + to_string(strnum), "1", "str");
            strnum ++;
            if (symtype == COMMASY) { // string + expression
                nxtsym();
                expression(true);
                emit(write, to, to_string(ischar), "");
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
            idx = searchtab(token, variable);
            if (idx == 0) {
                error();
                return;
            }
            string typl = stab.element[idx].type == ints ? "int" : "char";
            bool isglobal = stab.element[idx].level == 1;
            emit(read, token, typl, "", isglobal, stab.element[idx].address); // TODO: or use register?
            nxtsym();
            if (symtype == COMMASY) {
                do {
                    nxtsym();
                    idx = searchtab(token, variable);
                    if (idx == 0) {
                        error();
                        return;
                    }
                    typl = stab.element[idx].type == ints ? "int" : "char";
                    isglobal = stab.element[idx].level == 1;
                    emit(read, token, typl, "", isglobal, stab.element[idx].address);
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
            expression(false);
            if (symtype != RPARSY) {
                error();
                return;
            }
            nxtsym();
        }
        emit(ret, to, "", "");
    }
    else {
        error();
        return;
    }
    cout << "This is a return statement." << endl;
}
