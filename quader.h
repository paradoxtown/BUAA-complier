//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_QUAD_H
#define COMPLIER_QUAD_H
#include "main.h"
#include <iostream>

using namespace std;

typedef struct {
    operation op;
    string rst;
    string opnum1;
    string opnum2;
}quad;

void quaderinit();
void emit(operation op, string rst, string num1, string num2);
void printquads();

#endif //COMPLIER_QUAD_H
