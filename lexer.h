//
// Created by MUSE on 2018/11/13.
//
#ifndef COMPLIER_LEXER_H
#define COMPLIER_LEXER_H
#define MAXN 100
#define LENGTH 200
#define ROWLENGTH 100000
#define SYMNUM 5

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

typedef struct {
    symbol sym;
    char name[MAXN];
    int rownum;
    int columnnum; // cc
    int prev;
}syminfo;

void init(char *path);
void readcode();
char getchr();
symbol isReserved(char *word);
int transNum(char *word);
void lexerror();
void setinfo();
symbol returnsym();
char *returnname();
void print(char *str, symbol sym);
void getsymInit();
int getsym();
void back(int symnumber);

#endif //COMPLIER_LEXER_H
