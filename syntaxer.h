//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_SYNTAXER_H
#define COMPLIER_SYNTAXER_H
#include "lexer.h"
#include "main.h"
#include "error.h"
#include "quader.h"
#include <string.h>
#include <iostream>
#include <vector>
#define NAMELEN 100
#define ELELEN 500
#define PINDEXLEN 500


typedef struct {
    char name[NAMELEN];
    obj object; // obj {constant, variable, typel, function};
    typ type; // typ {ints, chars, voids};
    int level;
    int value;
    int address; // the storage address of symbol or the offset
    int number; // means the number of parameter of function or the length of array
    bool hasvalue = false;
}item;

typedef struct {
    item element[ELELEN];
    int top; // the top pointer of symbol stack
    int pnum; // the total number of the sub programs
    int pindex[PINDEXLEN]; // index of sub program
}tab;

typedef struct {
    vector<typ> valuelist;
}params;

class Syntaxer {
public:
    void nxtsym();
    void enter(char *name, obj object, typ type, int lev, int value, int addr, int num);
    void pushtab(char *name, obj object, typ type, int lev, int value, int addr, int num);
    int  searchtab(char *name, obj object);
    void retfuncdec();
    void voidfuncdec();
    void functionmain();
    void param(string fname);
    void parameterlist(string fname);
    void valuelist(string fname);
    void characterlist();
    void ischaracter();
    void isnumber();
    void constantdef();
    void constdec();
    void vardef();
    void vardec();
    void program();
    void statement();
    void expression(bool lock);
    void term(bool lock);
    void factor(bool lock);
    void callvoidfunc(string funcname, int ix);
    void callretfunc(string funcname, int ix);
    void assignment();
    void statementlist();
    void compoundstatement();
    void ifstatement();
    void condition();
    void whilestatement();
    void switchstatement();
    void caselist(typ expression1);
    void casestatment(typ expression1);
    void defaultstatemnt();
    void printfstatment();
    void scanfstatement();
    void returnstatement();
    // void printtab();
};


#endif //COMPLIER_SYNTAXER_H
