//
// Created by MUSE on 2018/11/17.
//

#include "asmer.h"
#include "main.h"
#include <iostream>
#include <cstring>
#include <vector>

using namespace std;
int sp = 0x7fffeffc, fb = 0; // stack pointer
int spoffset = 0, varoffset = 0;
int regidx = 0;
string t3, t2, t1;
string curfuncname;
int offset1 = 0, offset2 = 0;
vector<int> offsetstack;
vector<string> functionname;
int depth = 0;

void initdata(quad midcode) {
    if (midcode.op == var) {
        cout << midcode.opnum1 << ":\t.word 0" << endl;
    }
    else if (midcode.op == cons) {
        cout << midcode.opnum1 << ":\t.word " << midcode.opnum2 << endl;
    }
    else if (midcode.op == arr) {
        cout << midcode.opnum1 << ":\t.word 0: " << midcode.opnum2 << endl;
    }
    else if (midcode.op == str) {
        cout << midcode.opnum1 << ":\t.asciiz " << midcode.rst << endl;
    }
}

int getidx(string reg) {
    return stoi(reg.substr(1, reg.size() - 1));
}

bool isconst(string opnum){
    char *head = (char *)opnum.c_str();
    return ('0' <= head[0] && head[0] <= '9');
}

void quad2asm() {
    bool isdata = true;
    cout << ".data" << endl;
    cout << R"(enter:   .asciiz "\n")" << endl;
    for (int i = 0; i < quadlist.size(); i ++) {
        if (isdata) { // .data
            while (quadlist[i].isglobal) {
                initdata(quadlist[i]);
                i ++;
            }
            i --;
            isdata = false;
            cout << ".text" << endl;
        }
        else { // .text
            if (quadlist[i].op == label) {
                cout << quadlist[i].rst << ":" << endl;
            }
            else if (quadlist[i].op == cons) { // const int a value
                t3 = "$t" + to_string(regidx % 3);
                regidx ++;
                int offset = quadlist[i].offset;
                cout << "li\t" << t3 << ", " << quadlist[i].opnum2 << endl;
                cout << "sw\t" << t3 << ", -" << offset << "($sp)" << endl;
            }
            else if (quadlist[i].op == fact) {
                int offset;
                if (isconst(quadlist[i].opnum2)) { // fact register variable length
                    offset = stoi(quadlist[i].opnum2) * 4;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    if (quadlist[i].opnum1 == "$v0") {
                        cout << "add\t" << t3 << ", $0, $v0" << endl;
                    }
                    else {
                        if (quadlist[i].isglobal) { // var is global
                            cout << "la\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                            cout << "add\t" << t3 << ", " << t3 << ", " << offset << endl;
                            cout << "lw\t" << t3 << ", -0(" << t3 << ")" << endl;
                        } else { // is the local, so we need know the basic address of function
                            offset += quadlist[i].offset;
                            cout << "lw\t" << t3 << ", -" << offset << "($sp)" << endl;
                        }
                    }
                }
                else if(!quadlist[i].opnum2.empty()) {
                    t2 = "$t" + to_string((regidx - 1) % 3);
                    cout << "mul\t" << t2 << ", " << t2 << ", 4" << endl;
                    if (quadlist[i].isglobal) {
                        t3 = "$t" + to_string(regidx % 3);
                        regidx ++;
                        cout << "la\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                        cout << "add\t" << t2 << ", " << t2 << ", " << t3 << endl;
                    }
                    else {
                        offset = quadlist[i].offset;
                        cout << "add\t" << t2 << ", " << t2 << ", " << offset << endl;
                        cout << "add\t" << t2 << ", " << t2 << ", " << "$sp" << endl; // t2 = t2 * 4 + offset + sp
                    }
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "lw\t" << t3 << ", 0(" << t2 << ")" << endl;
                }
                else { // fact register variable
                    offset = quadlist[i].offset;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    if (quadlist[i].opnum1 == "$v0") {
                        cout << "add\t" << t3 << ", $0, $v0" << endl;
                    }
                    else {
                        if (quadlist[i].isglobal) { // var is global
                            cout << "lw\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                        } else { // is the local, so we need know the basic address of function
                            cout << "lw\t" << t3 << ", -" << to_string(offset) << "($sp)" << endl;
                        }
                    }
                }
                // first we need lw the value of local var from the memory,
                // second we sw the value to the memory which is the tmp var
                offset = getidx(quadlist[i].rst) * 4; // offset should be the
                cout << "sw\t" << t3 << ", -" << to_string(offset) << "($s0)" << endl;
            }
            else if (quadlist[i].op == add) {
                // t2 = t1 + t0 // t2 = t1 + 1 // t2 = 1 + t1 // t2 = 1 + 2
                bool isconst1 = isconst(quadlist[i].opnum1), isconst2 = isconst(quadlist[i].opnum2);
                if (isconst1 && isconst2) {
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                    cout << "add\t" << t3 << ", " << t3 << ", " << quadlist[i].opnum2 << endl;
                    // we need to save the value of rst
                }
                else if(isconst1) {
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum2) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "add\t" << t3 << ", " << t3 << ", " << quadlist[i].opnum1 << endl;
                }
                else if(isconst2) {
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "add\t" << t3 << ", " << t3 << ", " << quadlist[i].opnum2 << endl;
                }
                else {
                    t2 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum2) * 4;
                    cout << "lw\t" << t2 << ", -" << offset << "($s0)" << endl;
                    offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "add\t" << t3 << ", " << t3 << ", " << t2 << endl;
                }
                int offset = getidx(quadlist[i].rst) * 4;
                cout << "sw\t" << t3 << ", -" << to_string(offset) << "($s0)" << endl;
            }
            else if (quadlist[i].op == sub) {
                bool isconst1 = isconst(quadlist[i].opnum1), isconst2 = isconst(quadlist[i].opnum2);
                if (isconst1 && isconst2) {
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                    cout << "sub\t" << t3 << ", " << t3 << ", " << quadlist[i].opnum2 << endl;
                }
                else if(isconst1) {
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum2) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "sub\t" << t3 << ", " << t3 << ", " << quadlist[i].opnum1 << endl;
                    cout << "sub\t" << t3 << ", " << "$0" << ", " << t3 << endl;
                }
                else if(isconst2) {
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "sub\t" << t3 << ", " << t3 << ", " << quadlist[i].opnum2 << endl;
                }
                else {
                    string pret3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum2) * 4;
                    cout << "lw\t" << pret3 << ", -" << offset << "($s0)" << endl;
                    offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "sub\t" << t3 << ", " << t3 << ", " << pret3 << endl;
                }
                int offset = getidx(quadlist[i].rst) * 4;
                cout << "sw\t" << t3 << ", -" << to_string(offset) << "($s0)" << endl;
            }
            else if (quadlist[i].op == mult) { // c = a * b
                bool isconst1 = isconst(quadlist[i].opnum1), isconst2 = isconst(quadlist[i].opnum2);
                if (isconst1 && isconst2) {
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                    cout << "mul\t" << t3 << ", " << t3 << ", " << quadlist[i].opnum2 << endl;
                }
                else if(isconst1) {
                    t3 = "$t" + to_string(regidx % 3); // c
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum2) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl; // opnum2
                    cout << "mul\t" << t3 << ", " << t3 << ", " << quadlist[i].opnum1 << endl;
                }
                else if(isconst2) {
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "mul\t" << t3 << ", " << t3 << ", " << quadlist[i].opnum2 << endl;
                }
                else {
                    t2 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t2 << ", -" << offset << "($s0)" << endl;
                    offset = getidx(quadlist[i].opnum2) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "mul\t" << t3 << ", " << t3 << ", " << t2 << endl;
                }
                int offset = getidx(quadlist[i].rst) * 4;
                cout << "sw\t" << t3 << ", -" << to_string(offset) << "($s0)" << endl;
            }
            else if (quadlist[i].op == divide) {
                bool isconst1 = isconst(quadlist[i].opnum1), isconst2 = isconst(quadlist[i].opnum2);
                if (isconst1 && isconst2) {
                    t3 = "$t" + to_string(regidx % 3); // c
                    regidx ++;
                    cout << "li\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                    cout << "div\t" << t3 << ", " << quadlist[i].opnum2 << endl;
                }
                else if(isconst1) {
                    t2 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    t3 = "$t" + to_string(regidx % 3); // c
                    regidx ++;
                    cout << "li\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                    int offset = getidx(quadlist[i].opnum2) * 4;
                    cout << "lw\t" << t2 << ", -" << offset << "($s0)" << endl;
                    cout << "div\t" << t3 << ", " << t3 << ", " << t2 << endl;
                }
                else if(isconst2) {
                    t3 = "$t" + to_string(regidx % 3); // c
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "div\t" << t3 << ", " << t3 << ", " << quadlist[i].opnum2 << endl;
                }
                else {
                    t2 = "$t" + to_string(regidx % 3); // c
                    regidx ++;
                    t3 = "$t" + to_string(regidx % 3); // c
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    offset = getidx(quadlist[i].opnum2) * 4;
                    cout << "lw\t" << t2 << ", -" << offset << "($s0)" << endl;
                    cout << "div\t" << t3 << ", " << t3 << ", " << t2 << endl;
                }
                int offset = getidx(quadlist[i].rst) * 4;
                cout << "sw\t" << t3 << ", -" << to_string(offset) << "($s0)" << endl;
            }
            else if (quadlist[i].op == beq || quadlist[i].op == bne) { // beq rst, opnum, label
                string inst = quadlist[i].op == beq ? "beq\t" : "bne\t";

                bool isconst1 = isconst(quadlist[i].rst), isconst2 = isconst(quadlist[i].opnum1);
                if (isconst1 && isconst2) {
                    string pret3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << pret3 << ", " << quadlist[i].rst << endl;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                    cout << inst << pret3 << ", " << t3 << ", " << quadlist[i].opnum2 << endl;
                }
                else if(isconst1) {
                    string pret3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << pret3 << ", " << quadlist[i].rst << endl;
                    string t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << inst << pret3 << ", " << t3 << ", " << quadlist[i].opnum2 << endl;
                }
                else if(isconst2) {
                    string pret3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << pret3 << ", " << quadlist[i].opnum1 << endl;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].rst) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << inst << t3 << ", " << pret3 << ", " << quadlist[i].opnum2 << endl;
                }
                else {
                    string pret3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].rst) * 4;
                    cout << "lw\t" << pret3 << ", -" << offset << "($s0)" << endl;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << inst << pret3 << ", " << t3 << ", " << quadlist[i].opnum2 << endl;
                }
            }
            else if (quadlist[i].op == bltz || quadlist[i].op == blez
                    || quadlist[i].op == bgtz || quadlist[i].op == bgez) { // bgez rst opnum label
                string inst; // bgez (rst - opnum) label
                switch(quadlist[i].op) {
                    case bltz:
                        inst = "bltz\t";
                        break;
                    case blez:
                        inst = "blez\t";
                        break;
                    case bgtz:
                        inst = "bgtz\t";
                        break;
                    case bgez:
                        inst = "bgez\t";
                        break;
                    default:;
                }

                bool isconst1 = isconst(quadlist[i].rst), isconst2 = isconst(quadlist[i].opnum1);
                if (isconst1 && isconst2) {
                    string pret3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << pret3 << ", " << quadlist[i].rst << endl;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                    cout << "sub\t" << t3 << ", " << pret3 << ", " << t3 << endl;
                }
                else if(isconst1) {
                    string pret3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << pret3 << ", " << quadlist[i].rst << endl;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "sub\t" << t3 << ", " << pret3 << ", " << t3 << endl;
                }
                else if(isconst2) {
                    string pret3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << pret3 << ", " << quadlist[i].opnum1 << endl;
                    t3 = "$t"  + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].rst) * 4;
                    cout << "lw\t" << t3 << ", -" << offset << "($s0)" << endl;
                    cout << "sub\t" << t3 << ", " << t3 << ", " << pret3 << endl;
                }
                else {
                    string pret3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    int offset = getidx(quadlist[i].rst) * 4;
                    cout << "lw\t" << pret3 << ", -" << "($s0)" << endl;
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    offset = getidx(quadlist[i].opnum1) * 4;
                    cout << "lw\t" << t3 << ", -" << "($s0)" << endl;
                    cout << "sub\t" << t3 << ", " << pret3 << ", " << t3 << endl;
                }
                cout << inst << t3 << ", " << quadlist[i].opnum2 << endl;
            }
            else if (quadlist[i].op == assign) {
                if (!quadlist[i].opnum2.empty()) { // assign str tn n
                    int offset = getidx(quadlist[i].opnum1) * 4;
                    string pret3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "lw\t" << pret3 << ", -" << offset << "($s0)" << endl;
                    cout << "mul\t" << pret3 << ", " << pret3 << ", 4" << endl;
                    if (quadlist[i].isglobal) {
                        t3 = "$t" + to_string(regidx % 3);
                        regidx ++;
                        cout << "la\t" << t3 << ", " << quadlist[i].rst << endl;
                        cout << "add\t" << pret3 << ", " << pret3 << ", " << t3 << endl;
                    }
                    else {
                        offset = quadlist[i].offset;
                        cout << "add\t" << pret3 << ", " << pret3 << ", " << offset << endl;
                        cout << "add\t" << pret3 << ", " << pret3 << ", $sp" << endl;
                    }
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    if (isconst(quadlist[i].opnum2)) {
                        cout << "li\t" << t3 << ", " << quadlist[i].opnum2 << endl;
                    }
                    else {
                        offset = getidx(quadlist[i].opnum2);
                        cout << "lw\t" << t3 << ", " << offset << "($s0)" << endl;
                    }
                    cout << "sw\t" << t3 << ", 0(" << pret3 << ")" << endl;
                }
                else {
                    int offset = quadlist[i].offset;
                    if (isconst(quadlist[i].opnum1)) { // assign a = 1
                        t3 = "$t" + to_string(regidx % 3);
                        regidx++;
                        cout << "li\t" << t3 << ", " << quadlist[i].opnum1 << endl; // li t3 1
                        if (quadlist[i].isglobal) {
                            cout << "la\t$a0, " << quadlist[i].rst << endl;
                            cout << "sw\t" << t3 << ", " << "0($a0)" << endl;
                        } else cout << "sw\t" << t3 << ", -" << to_string(offset) << "($sp)" << endl;
                    } else { // assign a = b
                        t2 = "$t" + to_string((regidx - 1) % 3);
                        if (quadlist[i].isglobal) {
                            cout << "la\t$a0, " << quadlist[i].rst << endl;
                            cout << "sw\t" << t2 << ", " << "0($a0)" << endl;
                        } else cout << "sw\t" << t2 << ", -" << to_string(offset) << "($sp)" << endl;
                    }
                }
            }
            else if (quadlist[i].op == jump) {
                cout << "j\t" << quadlist[i].rst << endl;
            }
            else if (quadlist[i].op == call) {
                // the offset of tmp variable
                // offset = local var offset + tmp var offset
                offset1 += tmpvar[curfuncname] * 4 + localvar[curfuncname] * 4 + 4 * depth;
                offset2 += tmpvar[curfuncname] * 4 + localvar[quadlist[i].rst] * 4 + 4 * depth;
                offsetstack.push_back(offset2);
                offsetstack.push_back(offset1);
                curfuncname = quadlist[i].rst;
                functionname.push_back(curfuncname);
                depth ++;
            }
            else if (quadlist[i].op == push) {
                offset1 = offsetstack.back();
                int offset = quadlist[i].offset + offset1 + 4; // the 4 is the ra space offset
                if (isconst(quadlist[i].rst)) {
                    // li t3 im number
                    // sw t3 offset($sp)
                    t3 = "$t" + to_string(regidx % 3);
                    regidx ++;
                    cout << "li\t" << t3 << ", " << quadlist[i].rst << endl;
                    cout << "sw\t" << t3 << ", -" << to_string(offset) << "($sp)" << endl;
                }
                else {
                    // sw t2 offset($sp)
                    if (quadlist[i].isglobal) {
                        t3 = "$t" + to_string(regidx);
                        regidx ++;
                        cout << "lw\t" << t3 << ", " << quadlist[i].opnum1 << endl;
                    }
                    else {
                        t2 = "$t" + to_string((regidx - 1) % 3);
                    }
                    cout << "sw\t" << t2 << ", -" << to_string(offset) << "($sp)" << endl;
                }
            }
            else if (quadlist[i].op == jal) {
                offset1 = offsetstack.back();
                offsetstack.pop_back();
                offset2 = offsetstack.back();
                offsetstack.pop_back();
                cout << "sub\t$sp, $sp, " << offset1 << endl; // it is the begin of the new function
                cout << "sw\t$ra, 0($sp)" << endl;
                // cout << "sub\t$sp, $sp, 4" << endl;
                cout << "jal\t" << curfuncname << endl;
                cout << "add\t$sp, $sp, 4" << endl;
                cout << "lw\t$ra, 0($sp)" << endl;
                cout << "add\t$sp, $sp, " << offset1 << endl;
                cout << "add\t$s0, $s0, " << (offset2 + 4) << endl;
                offset1 = 0;
                offset2 = 0;
                depth --;
                functionname.pop_back();
                curfuncname = functionname.back();
            }
            else if (quadlist[i].op == ret) {
                if (isconst(quadlist[i].rst)) {
                    cout << "li\t$v0, " << quadlist[i].rst << endl;
                }
                else {
                    int offset = getidx(quadlist[i].rst) * 4;
                    cout << "lw\t$v0, -" << offset << "($s0)" << endl;
                }
                cout << "jr\t$ra" << endl;
            }
            else if (quadlist[i].op == write) {
                if (quadlist[i].opnum1 == "1") {
                    if (quadlist[i].opnum2 == "str") {
                        cout << "la\t$a0, " << quadlist[i].rst << endl;
                        cout << "li\t$v0, 4" << endl;
                    }
                    else {
                        if (isconst(quadlist[i].rst)) {
                            cout << "li\t$a0, " << quadlist[i].rst << endl;
                        }
                        else {
                            int offset = getidx(quadlist[i].rst) * 4;
                            cout << "lw\t$a0, -" << offset << "($s0)" << endl;
                        }
                        cout << "li\t$v0, 11" << endl;
                    }
                }
                else {
                    if (isconst(quadlist[i].rst)) {
                        cout << "li\t$a0, " << quadlist[i].rst << endl;
                    }
                    else {
                        int offset = getidx(quadlist[i].rst) * 4;
                        cout << "lw\t$a0, -" << offset << "($s0)" << endl;
                    }
                    cout << "li\t$v0, 1" << endl;
                }
                cout << "syscall" << endl;
                cout << "la\t$a0, enter" << endl;
                cout << "li\t$v0, 4" << endl;
                cout << "syscall" << endl;
            }
            else if (quadlist[i].op == read) {
                if (quadlist[i].opnum1 == "int") {
                    cout << "li\t$v0, 5" << endl;
                }
                else cout << "li\t$v0, 12" << endl;
                cout << "syscall" << endl;
                int offset = quadlist[i].offset;
                if (quadlist[i].isglobal) {
                    cout << "la\t$a0, " << quadlist[i].rst << endl;
                    cout << "sw\t$v0" << ", " << "0($a0)" << endl;
                }
                else {
                    cout << "sw\t$v0" << ", -" << to_string(offset) << "($sp)" << endl;
                }
            }
            else if (quadlist[i].op == func) {
                curfuncname = quadlist[i].opnum1;
                functionname.push_back(curfuncname);
                int offset = localvar[quadlist[i].opnum1] * 4; // this is the number of parameter + local var
                cout << "sub\t$sp, $sp, 4" << endl; // alloc space for $ra
                cout << "sub\t$s0, $sp, " << offset << endl;
            }
        }
    }
}

void testoffset(){
    for (auto &i: tmpvar) {
        cout << i.first << " " << i.second << endl;
    }
    for (auto &i: localvar) {
        cout << i.first << " " << i.second << endl;
    }
}
