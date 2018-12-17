//
// Created by MUSE on 2018/11/17.
//

#include "optimizer.h"
#include "quader.h"
#include "tools.h"
#include <map>

using namespace std;

int bias = 0;

map<string, reg> regs;

void removesubexpression() {

}

void mergeconstant(){
    for (auto & midcode: quadlist) {
        if (midcode.op == func) {
            bias = 0;
            regs.clear();
        } else if (midcode.op == add) {
            if (isconst(midcode.opnum1) && isconst(midcode.opnum2)) {
                regs[midcode.rst].value = stoi(midcode.opnum1) + stoi(midcode.opnum2);
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (isconst(midcode.opnum1) && regs[midcode.opnum2].hasvalue) {
                regs[midcode.rst].value = stoi(midcode.opnum1) + regs[midcode.opnum2].value;
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (isconst(midcode.opnum2) && regs[midcode.opnum1].hasvalue) {
                regs[midcode.rst].value = stoi(midcode.opnum2) + regs[midcode.opnum1].value;
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (regs[midcode.opnum1].hasvalue && regs[midcode.opnum2].hasvalue) {
                regs[midcode.rst].value = regs[midcode.opnum1].value + regs[midcode.opnum2].value;
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (regs[midcode.opnum1].hasvalue) {
                midcode.opnum1 = to_string(regs[midcode.opnum1].value);
            } else if (regs[midcode.opnum2].hasvalue) {
                midcode.opnum2 = to_string(regs[midcode.opnum2].value);
            }
        } else if (midcode.op == sub) {
            if (isconst(midcode.opnum1) && isconst(midcode.opnum2)) {
                regs[midcode.rst].value = stoi(midcode.opnum1) - stoi(midcode.opnum2);
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (isconst(midcode.opnum1) && regs[midcode.opnum2].hasvalue) {
                regs[midcode.rst].value = stoi(midcode.opnum1) - regs[midcode.opnum2].value;
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (isconst(midcode.opnum2) && regs[midcode.opnum1].hasvalue) {
                regs[midcode.rst].value = regs[midcode.opnum1].value - stoi(midcode.opnum2);
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (regs[midcode.opnum1].hasvalue && regs[midcode.opnum2].hasvalue) {
                regs[midcode.rst].value = regs[midcode.opnum1].value - regs[midcode.opnum2].value;
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (regs[midcode.opnum1].hasvalue) {
                midcode.opnum1 = to_string(regs[midcode.opnum1].value);
            } else if (regs[midcode.opnum2].hasvalue) {
                midcode.opnum2 = to_string(regs[midcode.opnum2].value);
            }
        } else if (midcode.op == mult) {
            if (isconst(midcode.opnum1) && isconst(midcode.opnum2)) {
                regs[midcode.rst].value = stoi(midcode.opnum1) * stoi(midcode.opnum2);
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (isconst(midcode.opnum1) && regs[midcode.opnum2].hasvalue) {
                regs[midcode.rst].value = stoi(midcode.opnum1) * regs[midcode.opnum2].value;
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (isconst(midcode.opnum2) && regs[midcode.opnum1].hasvalue) {
                regs[midcode.rst].value = regs[midcode.opnum1].value * stoi(midcode.opnum2);
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (regs[midcode.opnum1].hasvalue && regs[midcode.opnum2].hasvalue) {
                regs[midcode.rst].value = regs[midcode.opnum1].value * regs[midcode.opnum2].value;
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (regs[midcode.opnum1].hasvalue) {
                midcode.opnum1 = to_string(regs[midcode.opnum1].value);
            } else if (regs[midcode.opnum2].hasvalue) {
                midcode.opnum2 = to_string(regs[midcode.opnum2].value);
            }
        } else if (midcode.op == divide) {
            if (isconst(midcode.opnum1) && isconst(midcode.opnum2)) {
                regs[midcode.rst].value = stoi(midcode.opnum1) / stoi(midcode.opnum2);
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (isconst(midcode.opnum1) && regs[midcode.opnum2].hasvalue) {
                regs[midcode.rst].value = stoi(midcode.opnum1) / regs[midcode.opnum2].value;
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (isconst(midcode.opnum2) && regs[midcode.opnum1].hasvalue) {
                regs[midcode.rst].value = regs[midcode.opnum1].value / stoi(midcode.opnum2);
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (regs[midcode.opnum1].hasvalue && regs[midcode.opnum2].hasvalue) {
                regs[midcode.rst].value = regs[midcode.opnum1].value / regs[midcode.opnum2].value;
                regs[midcode.rst].hasvalue = true;
                bias++;
                midcode.op = null;
            } else if (regs[midcode.opnum1].hasvalue) {
                midcode.opnum1 = to_string(regs[midcode.opnum1].value);
            } else if (regs[midcode.opnum2].hasvalue) {
                midcode.opnum2 = to_string(regs[midcode.opnum2].value);
            }
        } else if (midcode.op == bltz || midcode.op == blez
                   || midcode.op == bgtz || midcode.op == bgez
                   || midcode.op == beq || midcode.op == bne) {
            // option rst, opnum1, label
            // TODO
            if (regs[midcode.rst].hasvalue) {
                midcode.rst = to_string(regs[midcode.rst].value);
            }
            if (regs[midcode.opnum1].hasvalue) {
                midcode.opnum1 = to_string(regs[midcode.opnum1].value);
            }
        }
        else if (midcode.op == assign) {
            if (regs[midcode.opnum1].hasvalue) {
                midcode.opnum1 = to_string(regs[midcode.opnum1].value);
            }
            if (regs[midcode.opnum2].hasvalue) {
                midcode.opnum2 = to_string(regs[midcode.opnum2].value);
            }
        }
        else if (midcode.op == push) {
            if (regs[midcode.rst].hasvalue) {
                midcode.rst = to_string(regs[midcode.rst].value);
            }
        }
    }
}


