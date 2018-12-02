#include <utility>

//
// Created by MUSE on 2018/11/17.
//
#include "quader.h"
#include "main.h"
#include <map>
#include <vector>

map<operation, string> op2str;
vector<quad> quadlist;

void quaderinit(){
    op2str[cons] = "const";
    op2str[var] = "var";
    op2str[arr] = "array";
    op2str[str] = "string";
    op2str[func] = "func";
    op2str[para] = "para";
    op2str[assign] = "assign";
    op2str[add] = "add";
    op2str[divide] = "div";
    op2str[mult] = "mult";
    op2str[sub] = "sub";
    op2str[fact] = "factor";
    op2str[bltz] = "bltz";
    op2str[blez] = "blez";
    op2str[bgtz] = "bgtz";
    op2str[bgez] = "bgez";
    op2str[bne] = "bne";
    op2str[beq] = "beq";
    op2str[jump] = "jump";
    op2str[label] = "label";
    op2str[push] = "push";
    op2str[call] = "call";
    op2str[read] = "read";
    op2str[write] = "write";
    op2str[ret] = "ret";
    op2str[jal] = "jal";
}

void emit(operation op, string rst, string num1, string num2) {
    quad midcode;
    midcode.op = op;
    midcode.rst = std::move(rst);
    midcode.opnum1 = std::move(num1);
    midcode.opnum2 = std::move(num2);
    quadlist.push_back(midcode);
}

void emit(operation op, string rst, string num1, string num2, bool isglobal, int offset) {
    quad midcode;
    midcode.op = op;
    midcode.rst = std::move(rst);
    midcode.opnum1 = std::move(num1);
    midcode.opnum2 = std::move(num2);
    midcode.isglobal = isglobal;
    midcode.offset = offset;
    if (op == str) {
        quadlist.insert(quadlist.begin(), midcode);
    }
    else quadlist.push_back(midcode);
}

void printquads(){
    for (auto &midcode : quadlist) {
        if (midcode.op == label) {
            cout << midcode.rst << endl;
        }
        else {
            cout << op2str[midcode.op] << "\t" << midcode.rst << " " << midcode.opnum1 << " " << midcode.opnum2 << endl;
        }
    }
}

void printformat(){
    vector<quad> tmpquads;
    for (int i = 0; i < quadlist.size(); i ++) {
        if (quadlist[i].op == para) {
            tmpquads.push_back(quadlist[i]);
        }
        else if (quadlist[i].op == func) {
            cout << quadlist[i].rst << "\t" << quadlist[i].opnum1 << "()" << endl;
            for (auto& parameter : tmpquads) {
                cout << "para " << parameter.rst << " " << parameter.opnum1 << endl;
            }
            tmpquads.clear();
        }
        else if (quadlist[i].op == call) {
            cout << "call" << quadlist[i].rst << endl;
            cout << quadlist[i + 1].rst << " = RET" << endl;
        }
        else if (quadlist[i].op == cons) {
            cout << "const " << quadlist[i].rst << quadlist[i].opnum1 << " = " << quadlist[i].opnum2 << endl;
        }
        else if (quadlist[i].op == add || quadlist[i].op == sub) {

        }
    }
}

string getname(string reg) {
    int index = stoi(reg.substr(1, reg.size() - 1));
    return t[index].empty() ? reg : t[index];
}