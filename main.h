//
// Created by MUSE on 2018/11/17.
//

#include "lexer.h"
#ifndef COMPLIER_COMPILER_H
#define COMPLIER_COMPILER_H

enum obj {constant, variable, typel, function, parameter, arrays};
enum typ {ints, chars, voids};
extern char token[MAXN];
extern symbol result;

#endif //COMPLIER_COMPILER_H
