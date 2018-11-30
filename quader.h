//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_QUAD_H
#define COMPLIER_QUAD_H
#include "main.h"
#include <iostream>
#include <vector>
#include <map>

using namespace std;

typedef struct {
    operation op;
    string rst;
    string opnum1;
    string opnum2;
    bool isglobal = false;
    int offset;
}quad;
extern vector<quad> quadlist;
extern map<operation, string> op2str;

void quaderinit();
void emit(operation op, string rst, string num1, string num2);
void emit(operation op, string rst, string num1, string num2, bool isglobal, int offset);
void printquads();
void printformat();

#endif //COMPLIER_QUAD_H
