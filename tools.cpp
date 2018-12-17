//
// Created by MUSE on 2018/12/14.
//

#include "tools.h"

int getidx(string reg) {
    return stoi(reg.substr(1, reg.size() - 1));
}

bool isconst(string opnum){
    char *head = (char *)opnum.c_str();
    return ('0' <= head[0] && head[0] <= '9') || (head[0] == '-' && '0' <= head[1] && head[1] <= '9');
}