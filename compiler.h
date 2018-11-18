//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_COMPILER_H
#define COMPLIER_COMPILER_H

typedef struct{
    char name[50];
    int type;
    int value;
    int address;
    int num;
}item;

typedef struct{
    item element[100];
    int index;
    int pnum;
    int pindex[100];
}tab;

#endif //COMPLIER_COMPILER_H
