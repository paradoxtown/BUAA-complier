//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_OPTIMIZER_H
#define COMPLIER_OPTIMIZER_H
#include "main.h"
#include "quader.h"

typedef struct{
    int value;
    bool hasvalue = false;
}reg;

extern map<string, reg> regs;
void removesubexpression();
void mergeconstant();
void globalmergeconstant();

#endif //COMPLIER_OPTIMIZER_H
