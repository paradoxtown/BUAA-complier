//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_ERROR_H
#define COMPLIER_ERROR_H
#include <iostream>
#include "main.h"

using namespace std;

enum errorstate{
    HASBEENDEFINED,
    NOTBEENDEFINED,
    ERRORNUM,
    ERRORRETVALUE,
    NOTLPAR,
    NOTRPAR,
    NOTLPRT,
    NOTRPRT,
    NOTLBRA,
    NOTRBRA,
    NOTVOID,
    NOTMAIN,
    NOTDQUO,
    NOTSQUO,
    NOTPLUS,
    NOTMULT,
    NOTASSIGN,
    NOTIDENT,
    NOTTYP,
    NOTCONST,
    NOTSEMI,
    NOTINTEGER,
    NOTCHAR,
    NOMAIN,
    ERRORRET,
    ERRORRECORD,
    ERRORSTATEMENT,
    NOTIF,
    NOTCONDITION,
    NOTLOOP,
    NOTSWITCH,
    NOTCASE,
    NOTDEFAULT,
    NOTPRINT,
    NOTSCAN,
    NOTRET,
    NOTCOLON,
    OUTSTACK,
    ERRORDEFAULT,
    TYPEDONTMATCH,
    PARATYPENOTMATCH
};

void errorinit();
void error();
void error(errorstate state, int line);
void test();

// about return
#endif //COMPLIER_ERROR_H
