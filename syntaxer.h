//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_SYNTAXER_H
#define COMPLIER_SYNTAXER_H
#include "lexer.h"
#include "main.h"
#define NAMELEN 50
#define ELELEN 100
#define PINDEXLEN 100

typedef struct{
    char name[NAMELEN];
    obj object; // obj {constant, variable, typel, function};
    typ type; // typ {ints, chars, voids};
    int lev;
    int value;
    int address; // the storage address of symbol or the offset
    int num; // means the number of parameter of function or the length of array
}item;

typedef struct{
    item element[ELELEN];
    int top; // the top pointer of symbol stack
    int pnum; // the total number of the sub programs
    int pindex[PINDEXLEN]; // index of sub program
}tab;

class Syntaxer {
public:
    void nxtsym();
    void enter(char *name, obj object, typ type, int lev, int value, int addr, int num);
    void pushtab(char *name, obj object, typ type, int lev, int value, int addr, int num);
    int  searchtab(char *name, obj object);
    int  searchtab(char *name);
    void retfuncdec();
    void voidfuncdec();
    void functionmain();
    void param();
    void parameterlist();
    void valuelist();
    void characterlist();
    void ischaracter();
    void isnumber();
    void constantdef();
    void constdec();
    void dechead();
    void vardef();
    void vardec();
    void program();
    void statement();
    void expression();
    void term();
    void factor();
    void callvoidfunc();
    void callretfunc();
    void assignment();
    void statementlist();
    void compoundstatement();
    void ifstatement();
    void condition();
    void whilestatement();
    void switchstatement();
    void caselist();
    void casestatment();
    void defaultstatemnt();
    void printfstatment();
    void scanfstatement();
    void returnstatement();
};


#endif //COMPLIER_SYNTAXER_H
