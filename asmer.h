//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_ASMER_H
#define COMPLIER_ASMER_H


class Asmer {
public:
    void push();
    void getaddr();
    void entercode();
    void constdeccode();
    void vardeccode();
    void callcode();
    void parametercode();
    void setlabel();
    void jumpcode();
    void assigncode();
    void whilecode();
    void ifcode();
    void switchcode();
    void scanfcode();
    void printfcode();
    void returncode();
    void quad2asm();
};

#endif //COMPLIER_ASMER_H
