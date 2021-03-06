//
// Created by MUSE on 2018/11/17.
//
#include "syntaxer.h"
#define BACK back(-- symnumber);
#define MAXTABLEN 512

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
bool ismain = false, isvoid = false, isover = false;
bool functyp;
symbol symtype;
char symname[NAMELEN];
char name4back[NAMELEN];
map<string, params> ptab;

void Syntaxer::nxtsym() {
    if (getsym() == 0) isover = true;
    symtype = result;
    symnumber ++;
    cout << token << endl;
}

void Syntaxer::enter(char *name, obj object, typ type, int level, int value, int address, int number) {
    if (stab.top > MAXTABLEN){
        error(OUTSTACK, linenumber);
    }
    pushtab(name, object, type, level, value, address, number);
}

void Syntaxer::pushtab(char *name, obj object, typ type, int level, int value, int address, int number){
    if (object == function){
        stab.pindex[stab.pnum ++] = stab.top + 1; // set this function to the stack's top
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
        emit(label, symname, "1", ""); // TODO: the function name shouldn't be same with the inst
        char funcname[NAMELEN];
        typ typl = symtype == INTSY ? ints : chars;
        functyp = typl;
        nxtsym();
        strcpy(funcname, token); // function name
        idx = searchtab(funcname, function);
        if (idx == 0) {
            enter(funcname, function, typl, level, 0, address, number); // we need to set space for function
        }
        else {
            error(HASBEENDEFINED, linenumber);
        }
        funcidx = stab.top - 1;
        nxtsym();
        if (symtype != LPARSY) {
            error(NOTLPAR, linenumber);
        }
        else nxtsym();
        parameterlist(funcname); // get para list length
        stab.element[funcidx].number = number; // attention on this funcidx, reset the number of function
        emit(func, typl == ints ? "int" : "char", funcname, to_string(number)); // we need to use emit2
        if (symtype != LPRTSY) {
            error(NOTLPRT, linenumber);
        }
        else nxtsym();
        compoundstatement();
        tmpvar[funcname] = regnum; // record the number of tmpvar
        localvar[funcname] = address / 4;
        if (symtype != RPRTSY) {
            error(NOTRPRT, linenumber);
            return;
        }
        stab.top = stab.pindex[stab.pnum - 1];
    }
    else {
        error(NOTTYP, linenumber);
        return;
    }
    nxtsym();
    cout << "This is a declaration of function which have returning value." << endl;
}

// ＜无返回值函数定义＞ ::= void＜标识符＞'('＜参数表＞')''{'＜复合语句＞'}'
void Syntaxer::voidfuncdec() {
    if (symtype == VOIDSY) {
        isvoid = true;
        address = 0;
        regnum = 0;
        emit(label, symname, "1", "");
        char funcname[NAMELEN];
        nxtsym();
        strcpy(funcname, token);
        strcpy(symname, token);
        idx = searchtab(symname, function);
        if (idx == 0) {
            enter(symname, function, voids, level, value=0, address, number);
        }
        else {
            error(HASBEENDEFINED, linenumber);
        }
        funcidx = stab.top - 1;
        nxtsym();
        if (symtype != LPARSY) {
            error(NOTLPAR, linenumber);
        }
        else nxtsym();
        parameterlist(funcname);
        stab.element[funcidx].number = number;
        emit(func, "void", funcname, to_string(number));
        if (symtype != LPRTSY) {
            error(NOTLPRT, linenumber);
        }
        else nxtsym();
        compoundstatement();
        tmpvar[funcname] = regnum;
        localvar[funcname] = address / 4;
        if (symtype != RPRTSY) {
            error(NOTRPRT, linenumber);
            return;
        }
        stab.top = stab.pindex[stab.pnum - 1];
        emit(jr, "$ra", "", "");
        isvoid = false;
    }
    else {
        error(NOTVOID, linenumber);
        return;
    }
    nxtsym();
    cout << "This is a declaration of function which don\'t have returning value." << endl;
}

// ＜主函数＞ ::= void main'('')''{'＜复合语句＞'}'
void Syntaxer::functionmain() {
    ismain = true;
    if (symtype == VOIDSY) {
        address = 0;
        regnum = 0;
        emit(label, "main", "", "");
        emit(func, "void", "main", "0");
        nxtsym();
        if (symtype != MAINSY) {
            error(NOTMAIN, linenumber);
        }
        else nxtsym();
        if (symtype != LPARSY) {
            error(NOTLPAR, linenumber);
        }
        else nxtsym();
        if (symtype != RPARSY) {
            error(NOTRBRA, linenumber);
        }
        else nxtsym();
        if (symtype != LPRTSY) {
            error(NOTLPRT, linenumber);
        }
        else nxtsym();
        compoundstatement();
        tmpvar["main"] = regnum;
        localvar["main"] = address / 4;
        if (symtype != RPRTSY) { // '}'
            error(NOTRPRT, linenumber);
            return;
        }
        emit(label, "end", "", "");
    }
    else {
        error(NOTVOID, linenumber);
        return;
    }
    cout << "This is a declaration of function main." << endl;
}

// ＜参数＞ ::=  ＜类型标识符＞＜标识符＞
void Syntaxer::param(string fname) {
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
            ptab[fname].valuelist.push_back(typl);
        }
        else {
            error(HASBEENDEFINED, linenumber);
        }
    }
    nxtsym();
    cout << "This is a parameter." << endl;
}

// ＜参数表＞ ::= ＜参数＞{,＜参数＞}| ＜空>
void Syntaxer::parameterlist(string fname) {
    number = 0;
    cout << "This is the head of parameterlist." << endl;
    if (symtype == INTSY || symtype == CHARSY) {
        param(fname);
        number ++;
        if (symtype == COMMASY) {
            do {
                nxtsym();
                param(fname);
                number ++;
            } while(symtype == COMMASY);
        }
        if (symtype != RPARSY) {
            error(NOTRPAR, linenumber);
            return;
        }
    }
    else if (symtype != RPARSY) {
        error(NOTRPAR, linenumber);
        return;
    }
    nxtsym();
    cout << "This is a parameterlist." << endl;
}

// ＜值参数表＞ ::= ＜表达式＞{,＜表达式＞}｜＜空＞
void Syntaxer::valuelist(string fname) {
    int valuenum = 0;
    if (symtype == RPARSY) {
        // empty, no emit
        return;
    }
    int x = 0;
    ischar = 0;
    expression(true);
    typ expression1 = ischar == 1 ? chars : ints;
    if (!ptab[fname].valuelist.empty() && ptab[fname].valuelist[x] != expression1) {
        error(PARATYPENOTMATCH, linenumber);
    }
    x ++;
    valuenum ++;
    int tmpaddr = 0; // be relative to the function's address in the stack, this is the offset
    emit(push, to, "", "", false, tmpaddr); // TODO: use to register which is the returning value from expression
    tmpaddr += 4;
    if (symtype == COMMASY) {
        do {
            nxtsym();
            ischar = 0;
            expression(true);
            expression1 = ischar == 1 ? chars : ints;
            if (!ptab[fname].valuelist.empty() && ptab[fname].valuelist[x] != expression1) {
                error(PARATYPENOTMATCH, linenumber);
            }
            x ++;
            valuenum ++;
            emit(push, to, "", "", false, tmpaddr);
            tmpaddr += 4;
        } while (symtype == COMMASY);
        if (valuenum != ptab[fname].valuelist.size()) {
            error(ERRORVALUENUM, linenumber);
        }
    }
    else if (symtype != RPARSY){
        error(NOTRPAR, linenumber);
    }
    cout << "This is a value list." << endl;
}

// ＜字符串＞ ::="｛十进制编码为32,33,35-126的ASCII字符｝"
void Syntaxer::characterlist() {
    cout << "The string is " << token << endl;
    int tmpidx = 0;
    if (token[0] == '\"') {
        int tokenpoint = 1;
        do {
            if (token[tokenpoint] == '\\') {
                tmpidx = tokenpoint;
            }
            tokenpoint ++;
        } while(token[tokenpoint] != '\"');
        if (tmpidx) {
            string tmpstr = token;
            tmpstr = tmpstr.substr(0, (unsigned long long) tmpidx) + "\\" +
                     tmpstr.substr((unsigned long long) tmpidx, (unsigned long long)(tokenpoint + 1 - tmpidx));
            emit(str, tmpstr, "_str" + to_string(strnum), "", true, 0);
        }
        else emit(str, token, "_str" + to_string(strnum), "", true, 0);
    }
    else {
        error(NOTDQUO, linenumber);
        return;
    }
    nxtsym();
    cout << "This is a character list." << endl;
}

// ＜字符＞ ::= '＜加法运算符＞'｜'＜乘法运算符＞'｜'＜字母＞'｜'＜数字＞'
void Syntaxer::ischaracter() {
    if (token[0] == '\''/*symtype == SQUOSY*/) { // "'"
        value = token[1];
        if (token[2] != '\''/* symtype == SQUOSY */) {
            error(NOTSQUO, linenumber);
        }
    }
}

// ＜整数＞ ::= ［＋｜－］＜无符号整数＞
void Syntaxer::isnumber() {
    if (symtype == PLUSSY || symtype == MINUSSY) { // "+" | "-"
        symbol tmpsym = symtype;
        nxtsym();
        value = transNum(token); // may i rename the "returnname" to " returntoken"?
        value = tmpsym == PLUSSY ? value : -value;
    } else if (symtype == INTEGERSY) {
        value = transNum(token);
    } else {
        error(NOTPLUS, linenumber);
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
                    if (idx == 0 || (level != 1 && stab.element[idx].level == 1)) {
                        number = 1;
                        enter(symname, constant, ints, level, value, address, number);
                        emit(cons, "int", symname, to_string(value), level == 1, address);
                        address += 4;
                    }
                    else {
                        error(HASBEENDEFINED, linenumber);
                    }
                    nxtsym();
                } else {
                    error(NOTASSIGN, linenumber);
                    do {
                        nxtsym();
                    } while(symtype != COMMASY || symtype != SEMISY);
                }
            }
            else {
                error(NOTIDENT, linenumber);
                do {
                    nxtsym();
                } while(symtype != COMMASY || symtype != SEMISY);
            }
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
                    if (idx == 0 || (level != 1 && stab.element[idx].level == 1)) {
                        number = 1;
                        enter(symname, constant, chars, level, value, address, number);
                        emit(cons, "char", symname, to_string(value), level == 1, address);
                        address += 4;
                    }
                    else {
                        error(HASBEENDEFINED, linenumber);
                    }
                    nxtsym();
                } else {
                    error(NOTASSIGN, linenumber);
                    do {
                        nxtsym();
                    } while (symtype != COMMASY || symtype != SEMISY);
                }
            }
            else {
                error(NOTIDENT, linenumber);
                do {
                    nxtsym();
                } while(symtype != COMMASY || symtype != SEMISY);
            }
        }while(symtype == COMMASY);
    }
    else {
        error(NOTTYP, linenumber);
        do {
            nxtsym();
        } while(symtype != SEMISY);
    }
    cout << "This is a constant define." << endl;
}

// ＜常量说明＞ ::=  const＜常量定义＞;{ const＜常量定义＞;}
void Syntaxer::constdec() {
    cout << "This the head of constdec" << endl;
    if (symtype != CONSTSY){ // the symbol before i came in
        error(NOTCONST, linenumber);
        do {
            nxtsym();
        } while(symtype != SEMISY);
    }
    else {
        nxtsym();
        constantdef();
        if (symtype != SEMISY){
            error(NOTSEMI, linenumber);
        }
        else nxtsym();
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
                        error(NOTINTEGER, linenumber);
                    }
                    else {
                        number = transNum(token); // the length of the array
                        nxtsym();
                    }
                    if (number <= 0){
                        error(ERRORNUM, linenumber);
                    }
                    if (symtype == RBRACKSY){ // ']'
                        value = 0;
                        idx = searchtab(symname, variable);
                        if (idx == 0 || (level != 1 && stab.element[idx].level == 1)) {
                            enter(symname, variable, tmpsym == INTSY ? ints : chars, level, value, address, number);
                            emit(arr, tmpsym == INTSY ? "int" : "char", symname, to_string(number), level == 1, address); // array
                            address += number * 4;
                        }
                        else {
                            error(HASBEENDEFINED, linenumber);
                        }
                        nxtsym();
                    }
                    else {
                        error(NOTRBRA, linenumber);
                    }
                }
                else { // is not array
                    value = 0;
                    number = 1;
                    idx = searchtab(symname, variable);
                    if (idx == 0 || (level != 1 && stab.element[idx].level == 1)) {
                        enter(symname, variable, tmpsym == INTSY ? ints : chars, level, value, address, number);
                        emit(var, tmpsym == INTSY ? "int" : "char", symname, "", level == 1, address);
                        address += 4;
                    }
                    else {
                        error(HASBEENDEFINED, linenumber);
                    }
                    // why needn't nxtsym, beacuse we have already nxtsymed before for judge array
                }
            }
            else {
                error(NOTIDENT, linenumber);
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
            error(NOTSEMI, linenumber);
        }
        else nxtsym();
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
            error(NOMAIN, linenumber);
        }
    }
    else {
        error(NOTTYP, linenumber);
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
        if (symtype != RPRTSY) { // '}'
            error(NOTRPRT, linenumber);
        }
        else nxtsym();
        return; // statement list needn't semi
    }
    else if (symtype == IDENTSY) {
        strcpy(name4back, token);
        strcpy(symname, token);
        nxtsym();
        if (symtype == ASSIGNSY || symtype == LBRACKSY) {
            BACK
            symtype = IDENTSY;
            idx = searchtab(symname, parameter);
            if (idx == 0) {
                idx = searchtab(symname, variable);
                if (idx == 0) {
                    error(NOTBEENDEFINED, linenumber);
                    do {
                        nxtsym();
                    } while(symtype != SEMISY);
                }
            }
            if (idx > 0) {
                assignment();
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
                    callvoidfunc(tmpname, funcidx); // call in statement
                }
                else {
                    callretfunc(tmpname, funcidx); // call just in statement
                }
            }
            else {
                error(NOTBEENDEFINED, linenumber);
                do {
                    nxtsym();
                } while(symtype != SEMISY);
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
        error(NOTSEMI, linenumber);
        return;
    }
    if (symtype != SEMISY) {
        error(NOTSEMI, linenumber);
        return;
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
            if (isover) return;
            statement();
            if (symtype == INTSY || symtype == CHARSY || symtype == CONSTSY) {
                error(ERRORSTRUCT, linenumber);
                do {
                    nxtsym();
                } while(symtype != SEMISY);
                nxtsym();
            }
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
            if (lock) ischar += 2; // TODO : it is a bug
            nxtsym();
            term(lock);
        }
        else { // - a + b
            if (lock) ischar += 2; // for - 'a'
            nxtsym();
            term(lock);
            if (isconst(to)) {
                to = to_string(-stoi(to));
            }
            else {
                string preto = to;
                to = "T" + to_string(regnum);
                regnum++;
                emit(sub, to, "0", preto); // to = 0 - tn
            }
        }
        string pretn = to;
        if (symtype == PLUSSY || symtype == MINUSSY) {
            do {
                symbol tmpsym = symtype;
                pretn = to;
                nxtsym();
                term(lock);
                string preto = to;
                to = "T" + to_string(regnum);
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
                to = "T" + to_string(regnum);
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
            pretn = to; // current to is the tn = "T" + to_string(regnum - 1); but we need the value if it is constant
            nxtsym();
            factor(lock);
            string preto = to;
            to = "T" + to_string(regnum);
            regnum ++;
            emit(tmpsym == MULTSY ? mult : divide, to, pretn, preto); // to = tn op tm
        } while (symtype == MULTSY || symtype == DIVSY);
    }
    cout << "This is a term." << endl;
}

// ＜因子＞ ::= ＜标识符＞｜＜标识符＞'['＜表达式＞']'|'('＜表达式＞')'｜＜整数＞|＜字符＞｜＜有返回值函数调用语句＞
void Syntaxer::factor(bool lock) {
    cout << "This is the head of factor." << endl;
    to = "T" + to_string(regnum);
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
                error(NOTBEENDEFINED, linenumber);
            } else if (idx > 0 && stab.element[stab.pindex[idx] - 1].type == voids) { // the returning value is from void
                error(ERRORRETVALUE, linenumber);
            }
            emit(call, tmpname, "", "");
            if(lock) stab.element[stab.pindex[idx] - 1].type == ints ? ischar += 2 : ischar ++;
            int valuenum = stab.element[stab.pindex[idx] - 1].number;
            nxtsym();
            int tmpischar = ischar;
            valuelist(tmpname);
            ischar = tmpischar;
            if (symtype != RPARSY) {
                error(NOTRPAR, linenumber);
            }
            else nxtsym();
            emit(jal, tmpname, to_string(valuenum), "");
            emit(fact, tmpto, "$v0", "");
            to = tmpto;
        }
        else if (symtype == LBRACKSY){ // array ==> factor tn array[x]
            regnum ++;
            idx = searchtab(symname, variable);
            if (idx == 0) {
                error(NOTBEENDEFINED, linenumber);
            }
            if(lock) stab.element[idx].type == ints ? ischar += 2 : ischar ++;
            int length = stab.element[idx].number;
            int tmpaddr = stab.element[idx].address;
            bool isglobal = stab.element[idx].level == 1;
            nxtsym();
            int tmpischar = ischar;
            ischar = 0;
            expression(true); // array[ expression ]
            if (ischar == 1) {
                error(NOTINTEGER, linenumber);
            }
            ischar = tmpischar;

            if (isconst(to)) {
                if (!(0 <= stoi(to) && stoi(to) <= length)) {
                    error(OUTSTACK, linenumber);
                }
            }

            if (symtype != RBRACKSY){ // missing ']'
                error(NOTRBRA, linenumber);
            }
            else nxtsym();
            emit(fact, tmpto, tmpname, to, isglobal, tmpaddr); // tn is the expression's value, fact to arr, i
            to = tmpto;
        }
        else { // identifier -> variable
            regnum ++;
            idx = searchtab(symname, parameter);
            if (idx == 0) {
                idx = searchtab(symname, variable) + searchtab(symname, constant);
                if (idx == 0) {
                    error(NOTBEENDEFINED, linenumber);
                }
            }
            if(lock) stab.element[idx].type == ints ? ischar += 2 : ischar ++;
            int tmpaddr = stab.element[idx].address;
            bool isglobal = stab.element[idx].level == 1;
            emit(fact, tmpto, tmpname, "", isglobal, tmpaddr); // fact to a
            to = tmpto;
            // why there is no nxtsym, cuz we have done it when we get in for judge wether it is function or array
        }
    }
    else if (symtype == LPARSY){ // '(' expression ')'
        nxtsym();
        expression(lock); // have getsym() at last
        if (symtype != RPARSY){ // ')'
            error(NOTRPAR, linenumber);
        }
        else nxtsym();
        if (lock) ischar += 2;
    }
    else if (symtype == INTEGERSY || symtype == MINUSSY || symtype == PLUSSY){ // [+/-] <integer>
        isnumber();
        nxtsym();
        if(lock) ischar += 2;
        to = to_string(value);
    }
    else if (symtype == SQUOSY){ // character
        ischaracter();
        nxtsym();
        if(lock) ischar ++;
        to = to_string(value);
    }
    else {
        error(NOTIDENT, linenumber);
    }
    cout << "This is a factor." << endl;
}

// ＜有返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Syntaxer::callretfunc(string funcname, int ix) {
    if (symtype == IDENTSY) {
        emit(call, funcname, "", "");
        int valuenum = stab.element[stab.pindex[ix] - 1].number;
        nxtsym();
        if (symtype != LPARSY) {
            error(NOTLPAR, linenumber);
        }
        else nxtsym();
        valuelist(funcname);
        if (symtype != RPARSY) {
            error(NOTRPAR, linenumber);
            return;
        }
        emit(jal, funcname, to_string(valuenum), "");
    }
    else {
        error(NOTIDENT, linenumber);
        return;
    }
    nxtsym();
    cout << "This is a call of returning function." << endl;
}

// ＜无返回值函数调用语句＞ ::= ＜标识符＞'('＜值参数表＞')'
void Syntaxer::callvoidfunc(string funcname, int ix) {
    if (symtype == IDENTSY) {
        emit(call, funcname, "", "");
        int valuenum = stab.element[stab.pindex[ix] - 1].number;
        nxtsym();
        if (symtype != LPARSY) {
            error(NOTLPAR, linenumber);
        }
        else nxtsym();
        valuelist(funcname);
        if (symtype != RPARSY) {
            error(NOTRPAR, linenumber);
            return;
        }
        emit(jal, funcname, to_string(valuenum), ""); // 1 means it is void
    }
    else {
        error(NOTIDENT, linenumber);
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
            ischar = 0;
            expression(true);
            typ expression1 = ischar == 1 ? chars : ints;
            idx = searchtab(name4back, parameter);
            if (idx == 0) {
                idx = searchtab(name4back, variable);
                if (idx == 0) {
                    error(NOTBEENDEFINED, linenumber);
                }
            }
            typ var1 = stab.element[idx].type;
            if (var1 != expression1) {
                error(TYPEDONTMATCH, linenumber);
            }
            int tmpaddr = stab.element[idx].address;
            bool isglobal = stab.element[idx].level == 1;
            emit(assign, name4back, to, "", isglobal, tmpaddr); // assign a tn
        }
        else if (symtype == LBRACKSY){ // is array '['
            nxtsym();
            ischar = 0;
            expression(true);
            typ expression1 = ischar == 1 ? chars : ints;
            if (expression1 != ints) {
                error(NOTINTEGER, linenumber);
            }
            string preto = to; // preto is the length, it may be the tn, or it may be the integer
            if (symtype != RBRACKSY){ // ']'
                error(NOTRBRA, linenumber);
            }
            else nxtsym();
            if (symtype == ASSIGNSY){
                nxtsym();
                ischar = 0;
                expression(true);
                typ expression2 = ischar == 1 ? chars : ints;
                idx = searchtab(name4back, variable);
                if (idx == 0) {
                    error(NOTBEENDEFINED, linenumber);
                }
                int length = stab.element[idx].number;
                if (isconst(preto)) {
                    if (!(0 <= stoi(preto) && stoi(preto) <= length)) {
                        error(OUTSTACK, linenumber);
                    }
                }
                typ array1 = stab.element[idx].type;
                if (array1 != expression2) {
                    error(TYPEDONTMATCH, linenumber);
                }
                int tmpaddr = stab.element[idx].address;
                bool isglobal = stab.element[idx].level == 1;
                emit(assign, name4back, preto, to, isglobal, tmpaddr); // assign a i b
            }
        }
    }
    else {
        error(NOTIDENT, linenumber);
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
            error(NOTLPAR, linenumber);
        }
        else nxtsym();
        condition();
        if (symtype != RPARSY) {
            error(NOTRPAR, linenumber);
        }
        else nxtsym();
        labeln = "label" + to_string(labelnum);
        labelnum ++;
        emit(jump, labeln, "", "");
        emit(label, labelm, "", "");
        string tmplabel = labeln;
        statement();
        // if statement also has label, so we should use the latest labelnum
        emit(label, tmplabel, "", "");
    }
    else {
        error(NOTIF, linenumber);
    }
    cout << "This is a \"if\" statement." << endl;

}

// ＜条件＞ ::= ＜表达式＞＜关系运算符＞＜表达式＞｜＜表达式＞ //表达式为0条件为假，否则为真
void Syntaxer::condition() {
    cout << "This is the head of condition." << endl;
    ischar = 0;
    expression(true);
    typ expression1 = ischar == 1 ? chars : ints;
    string preto = to;
    if (symtype == LSSY || symtype == LESY || symtype == GTSY ||
        symtype == GESY || symtype == NEQSY || symtype == EQUSY){
        symbol tmpsym = symtype;

        nxtsym();
        ischar = 0;
        expression(true);
        typ expression2 = ischar == 1 ? chars : ints;
//        if (expression1 != expression2) {
//            error(TYPEDONTMATCH, linenumber);
//        }
        if (expression1 == chars || expression2 == chars) {
            error(TYPEDONTMATCH, linenumber);
        }
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
        if (expression1 != ints) {
            error(TYPEDONTMATCH, linenumber);
        }
        labelm = "label" + to_string(labelnum);
        labelnum ++;
        emit(bne, preto, "0", labelm);
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
            error(NOTLPAR, linenumber);
        }
        else nxtsym();
        condition();
        if (symtype != RPARSY) {
            error(NOTRPAR, linenumber);
        }
        else nxtsym();
        emit(jump, whileout, "", ""); // if not equal
        emit(label, labelm, "", "");
        statement();
        emit(jump, beginlabel, "", "");
    }
    else {
        error(NOTLOOP, linenumber);
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
            error(NOTLPAR, linenumber);
        }
        else nxtsym();
        ischar = 0;
        expression(true);
        typ expression1 = ischar == 1 ? chars : ints;
        switchcase = to;
        if (symtype != RPARSY) {
            error(NOTRPAR, linenumber);
        }
        else nxtsym();
        if (symtype != LPRTSY) {
            error(NOTLPRT, linenumber);
        }
        else nxtsym();
        caselist(expression1);
        defaultstatemnt();
        if (symtype != RPRTSY) {
            error(NOTRPRT, linenumber);
            return;
        }
    }
    else {
        error(NOTSWITCH, linenumber);
        return;
    }
    nxtsym();
    emit(label, tmpswitchout, "", "");
    cout << "This is a switch statement." << endl;
}

// ＜情况表＞ ::= ＜情况子语句＞{＜情况子语句＞}
void Syntaxer::caselist(typ expression1) {
    casestatment(expression1);
    if (symtype == CASESY) {
        do {
            casestatment(expression1);
        } while (symtype == CASESY);
    }
    cout << "This is the case list." << endl;
}

// ＜情况子语句＞::=  case＜常量＞：＜语句＞
void Syntaxer::casestatment(typ expression1) {
    if (symtype == CASESY) {
        typ expression2;
        nxtsym();
        if (token[0] == '\'') {
            ischaracter(); // we don't use register in it, so we needn't to use tmp
            expression2 = chars;
        }
        else {
            isnumber();
            expression2 = ints;
        }
        if (expression1 != expression2) {
            error(TYPEDONTMATCH, linenumber);
        }
        nxtsym();
        if (symtype != COLONSY) { // ':'
            error(NOTCOLON, linenumber);
        }
        else nxtsym();
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
        statement();

        emit(jump, tmpswitchout, "", "");
        emit(label, tmplabel, "", "");
    }
    else {
        error(NOTCASE, linenumber);
        do {
            nxtsym();
        } while(symtype == CASESY);
    }
    cout << "This is a case sub statement." << endl;
}

// ＜缺省＞ ::=  default : ＜语句＞|＜空＞
void Syntaxer::defaultstatemnt() {
    cout << "This is the head of default statement." << endl;
    if (symtype == DEFAULTSY) {
        nxtsym();
        if (symtype != COLONSY) { // ':'
            error(NOTCOLON, linenumber);
        }
        else nxtsym();
        if (symtype != RPRTSY) { // '}'
            statement();
        }
        else { // default : } it is wrong
            error(ERRORDEFAULT, linenumber);
        }
    }
    else if (symtype != RPRTSY){ // empty
        error(NOTRPRT, linenumber);
    }
    cout << "This is a default statement." << endl;
}

// ＜写语句＞ ::= printf ('(' ＜字符串＞(,＜表达式＞|<空> ') | '('＜表达式＞')')
void Syntaxer::printfstatment() {
    ischar = 0;
    if (symtype == PRINTSY) {
        nxtsym();
        if (symtype != LPARSY) {
            error(NOTLPAR, linenumber);
        }
        else nxtsym();
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
            error(NOTRPAR, linenumber);
            return;
        }
    }
    else {
        error(NOTPRINT, linenumber);
        return;
    }
    nxtsym();
    cout << "This is a printf statement." << endl;
}

// ＜读语句＞ ::= scanf '('＜标识符＞{,＜标识符＞}')'
void Syntaxer::scanfstatement() {
    if (symtype == SCANFSY) {
        nxtsym();
        if (symtype == LPARSY) { // scanf(
            nxtsym();
            idx = searchtab(token, variable);
            if (idx == 0) {
                error(NOTBEENDEFINED, linenumber);
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
                        error(NOTBEENDEFINED, linenumber);
                    }
                    typl = stab.element[idx].type == ints ? "int" : "char";
                    isglobal = stab.element[idx].level == 1;
                    emit(read, token, typl, "", isglobal, stab.element[idx].address);
                    nxtsym();
                } while (symtype == COMMASY);
            }
        }
        if (symtype != RPARSY) { // ')'
            error(NOTRPAR, linenumber);
            return;
        }
    }
    else {
        error(NOTSCAN, linenumber);
        return;
    }
    nxtsym();
    cout << "This is a scanf statement." << endl;
}

// ＜返回语句＞ ::= return['('＜表达式＞')']
void Syntaxer::returnstatement() {
    if (symtype == RETSY) {
        nxtsym();
        if (symtype == LPARSY) { // return (
            if (isvoid) {
                error(ERRORRET, linenumber);
                do {
                    nxtsym();
                } while(symtype != SEMISY);
            }
            else {
                nxtsym();
                ischar = 0;
                expression(true);
                typ expression1 = ischar == 1 ? chars : ints;
                if (functyp != expression1) {
                    error(TYPEDONTMATCH, linenumber);
                }
                if (symtype != RPARSY) {
                    error(NOTRPAR, linenumber);
                } else nxtsym();
                emit(ret, to, "", "", ismain, 0);
            }
        }
        else if(symtype == SEMISY) { // return;
            emit(ret, "", "", "", ismain, 0);
        }
    }
    else {
        error(NOTRET, linenumber);
    }
    cout << "This is a return statement." << endl;
}