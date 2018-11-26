//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_MAIN_H
#define COMPLIER_MAIN_H
#include "lexer.h"

enum obj { constant, variable, typel, function, parameter, arrays };
enum typ { ints, chars, voids };
enum operation { cons, var, arr, func, para, assign, add, divide, mult, sub,
                 fact, ls, le, gt, ge, eq, neq, beq, jump, label, push, call,
                 read, write };
extern char token[MAXN];
extern symbol result;

#endif //COMPLIER_MAIN_H
