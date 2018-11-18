//
// Created by MUSE on 2018/11/17.
//

#ifndef COMPLIER_SYNTAXER_H
#define COMPLIER_SYNTAXER_H
#include "lexer.h"

class Syntaxer {
public:
    void enter();
    void funcdec();
    void constant();
    void parameterlist();
    void constdec();
    void typedec();
    void vardec();
    void progress();
    void statement();
    void expression();
    void term();
    void factor();
    void assignment();
    void statementlist();
    void compoundstatement();
    void ifstatement();
    void whilestatement();
    void switchstatement();
    void casestatment();
    void defaultstatemnt();
    void printfstatment();
    void scanfstatement();
    void returnstatement();
};


#endif //COMPLIER_SYNTAXER_H
