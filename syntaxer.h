//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_SYNTAXER_H
#define COMPLIER_SYNTAXER_H
#include "lexer.h"
#include "compiler.h"
#define NAMELEN 50
#define ELELEN 100
#define PINDEXLEN 100

typedef struct{
    char name[NAMELEN];
    obj object;
    typ type;
    int lev;
    int value;
    int address; // the storage address of symbol or the offset
    int num; // means the number of parameter of function or the length of array
}item;

typedef struct{
    item element[ELELEN];
    int index; // the top pointer of symbol stack
    int pnum; // the total number of the sub programs
    int pindex[PINDEXLEN]; // index of sub program
}tab;

class Syntaxer {
public:
    void enter(char *name, obj object, typ type, int lev, int value, int addr, int num);
    void pushtab(char *name, obj object, typ type, int lev, int value, int addr, int num);
    void funcdec();
    void constantdef();
    void parameterlist();
    void constdec();
    void typedec();
    void vardec();
    void progress();
    void statement();
    void expression();
    void term();
    void factor();
    void assignment();
    void statementlist();
    void compoundstatement();
    void ifstatement();
    void whilestatement();
    void switchstatement();
    void casestatment();
    void defaultstatemnt();
    void printfstatment();
    void scanfstatement();
    void returnstatement();
};


#endif //COMPLIER_SYNTAXER_H
