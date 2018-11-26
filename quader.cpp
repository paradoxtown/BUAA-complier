#include <utility>

//
// Created by MUSE on 2018/11/17.
//
#include "quader.h"
#include <map>
#include <vector>

map<operation, string> op2str;
vector<quad> quadlist;

void quaderinit(){
    op2str[cons] = "const";
    op2str[var] = "var";
    op2str[arr] = "array";
    op2str[func] = "func";
    op2str[para] = "para";
    op2str[assign] = "assign";
    op2str[add] = "add";
    op2str[divide] = "div";
    op2str[mult] = "mult";
    op2str[sub] = "sub";
    op2str[fact] = "fact";
    op2str[ls] = "ls";
    op2str[le] = "le";
    op2str[gt] = "gt";
    op2str[ge] = "ge";
    op2str[eq] = "eq";
    op2str[neq] = "neq";
    op2str[beq] = "beq";
    op2str[jump] = "jump";
    op2str[label] = "label";
    op2str[push] = "push";
    op2str[call] = "call";
    op2str[read] = "read";
    op2str[write] = "write";
}

void emit(operation op, string rst, string num1, string num2) {
    quad midcode;
    midcode.op = op;
    midcode.rst = std::move(rst);
    midcode.opnum1 = std::move(num1);
    midcode.opnum2 = std::move(num2);
    quadlist.push_back(midcode);
}

void printquads(){
    for (auto &midcode : quadlist) {
        cout << op2str[midcode.op] << "\t" << midcode.rst << " " << midcode.opnum1 << " " << midcode.opnum2 << endl;
    }
}

