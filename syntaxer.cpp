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
symbol symtype;
char symname[NAMELEN];

void Syntaxer::enter(char *name, obj object, typ type, int lev, int value, int addr, int num) {
    if (stab.index > MAXTABLEN){
        error(); // TODO
        return;
    }
    pushtab(name, object, type, lev, value, addr, num);
}

void Syntaxer::pushtab(char *name, obj object, typ type, int lev, int value, int addr, int num){
    if (object == function){
        for (int i = 0; i < stab.pnum; i ++){
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

// --------------------- syntaxer analysis -------------------
void Syntaxer::funcdec() {

}

// ＜constant(dec)＞ ::= int＜symbol＞＝＜integer＞{,＜symbol＞＝＜integer＞}|char＜标识符＞＝＜字符＞{,＜标识符＞＝＜字符＞}
void Syntaxer::constantdef() {
    // we must have met the "const" before
    getsym(); // TODO: should i read a symbol before i analysis it?
    symtype = returnsym();
    if (symtype == INTSY){ // if is "int symbol"
        getsym();
        symtype = returnsym();
        if (symtype == IDENTSY){ // symbol
            strcpy(symname, returnname());
            getsym();
            symtype = returnsym();
            if (symtype == ASSIGNSY){ // "="
                getsym();
                symtype = returnsym();
                if (symtype == PLUSSY || symtype == MINUSSY){ // "+" | "-"
                    getsym();
                    value = transNum(returnname()); // may i rename the "returnname" to " returntoken"?
                    value = symtype == PLUSSY ? value : - value;
                    addr ++; // TODO: i don't really understand the address and the num
                    num = - 1;
                    enter(symname, constant, ints, lev, value, addr, num);
                    // TODO: we will generate the 4-quad here in the future
                }
                else if (symtype == SQUOSY){ // "'"
                    // TODO: maybe we should optimize lexer
                    getsym();
                    symtype = returnsym(); // is a character
                    value = returnname()[0];
                    getsym();
                    if (returnsym() == SQUOSY){
                        addr ++;
                        num = 1;
                        enter(symname, constant, chars, lev, value, addr, num);
                        // TODO: we will generate the 4-quad here in the future
                    }
                }
                else if (symtype == INTEGERSY) {
                    //TODO: i forget to deal the first 0 problem
                    value = transNum(returnname());
                    addr++;
                    num = -1;
                    enter(symname, constant, ints, lev, value, addr, num);
                    // TODO: we will generate the 4-quad here in the future
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
    }
}

void Syntaxer::parameterlist() {

}

void Syntaxer::constdec() {

}

void Syntaxer::typedec() {

}

void Syntaxer::vardec() {

}

void Syntaxer::progress() {

}

void Syntaxer::statement() {

}

void Syntaxer::expression() {

}

void Syntaxer::term() {

}

void Syntaxer::factor() {

}

void Syntaxer::assignment() {

}

void Syntaxer::statementlist() {

}

void Syntaxer::compoundstatement() {

}

void Syntaxer::ifstatement() {

}

void Syntaxer::whilestatement() {

}

void Syntaxer::switchstatement() {

}

void Syntaxer::casestatment() {

}

void Syntaxer::defaultstatemnt() {

}

void Syntaxer::printfstatment() {

}

void Syntaxer::scanfstatement() {

}

void Syntaxer::returnstatement() {

}
