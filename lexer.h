//
// Created by MUSE on 2018/11/13.
//

#ifndef COMPLIER_LEXER_H
#define COMPLIER_LEXER_H


const char * const reserved_word[] = {
        "int",
        "char",
        "const",
        "void",
        "main",
        "switch",
        "case",
        "default",
        "if",
        "while",
        "scanf",
        "printf",
        "return"
};


enum symbol {
    PLUSSY,
    MINUSSY,
    MULTSY,
    DIVSY,
    LSSY,
    LESY,
    GTSY,
    GESY,
    NEQSY,
    EQUSY,
    ASSIGNSY,
    LPARSY,
    RPARSY,
    LBRACKSY,
    RBRACKSY,
    LPRTSY,
    RPRTSY,
    COLONSY,
    COMMASY,
    SEMISY,
    SQUOSY,
    DQUOSY,
    INTSY,
    CHARSY,
    CONSTSY,
    VOIDSY,
    MAINSY,
    SWITCHSY,
    CASESY,
    DEFAULTSY,
    IFSY,
    LOOPSY,
    SCANFSY,
    PRINTSY,
    RETSY,
    INTEGERSY,
    CHARACTERSY,
    STRINGSY,
    IDENTSY
};



void init(char *path);
char getchr();
symbol isReserved(char *word);
int transNum(char *word);
void error();
void print(char *str, symbol sym);
void getsymInit();
int getsym();

#endif //COMPLIER_LEXER_H
