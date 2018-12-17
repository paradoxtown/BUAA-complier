//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_ASMER_H
#define COMPLIER_ASMER_H
#include "quader.h"

void initdata(quad midcode);
void quad2asm();
int getidx(string reg);
bool isconst(string opnum);
void testoffset(quad * midcode);

#endif //COMPLIER_ASMER_H
