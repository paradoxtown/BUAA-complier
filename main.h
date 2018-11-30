//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_MAIN_H
#define COMPLIER_MAIN_H
#include "lexer.h"
#include <iostream>
#include <vector>
#include <map>

#define REGISTERNUM 1000000

using namespace std;

enum obj { constant, variable, function, arrays, parameter };
enum typ { ints, chars, voids };
enum operation { cons, var, arr, str, func, para, assign, add, divide, mult, sub,
                 fact, bltz, blez, bgtz, bgez, beq, bne, jump, label, push, call,
                 read, write, ret, jr, jal };
//enum inst {
//    madd,
//    maddi,
//    mbeq,
//    mdiv,
//    mj,
//    mjal,
//    mjr,
//    mlw,
//    mmfhi,
//    mmflo,
//    mmthi,
//    mmtlo,
//    mmult,
//    msw,
//    msll,
//    msub,
//    msyscall
//};

extern char token[MAXN];
extern map<string, int>tmpvar;
extern map<string, int>localvar;
extern symbol result;
const int hb = 0; // heap basic address
const int sb = 3000; // stack basic address
extern string t[REGISTERNUM];

#endif //COMPLIER_MAIN_H
