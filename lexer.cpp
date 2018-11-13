//
// Created by MUSE on 2018/11/13.
//
#include <iostream>
#include <cstring>
#include <cstdio>
#include <set>
#include <map>
#include "lexer.h"

#define MAXN 20
#define LENGTH 81

using namespace std;

char token[MAXN], line[LENGTH];
int num = 0;
int cc = 0, ll = 0, l = 0, cnt = 0;
FILE *fin, *fout;
symbol result;
map<string, symbol> key2sym;
map<symbol, string> sym2str;

void init(char *path){
    fin = fopen(path, "r");
    fout = fopen("./result.txt", "w");
    key2sym["int"] = INTSY;
    key2sym["char"] = CHARSY;
    key2sym["const"] = CONSTSY;
    key2sym["void"] = VOIDSY;
    key2sym["main"] = MAINSY;
    key2sym["switch"] = SWITCHSY;
    key2sym["case"] = CASESY;
    key2sym["default"] = DEFAULTSY;
    key2sym["if"] = IFSY;
    key2sym["while"] = LOOPSY;
    key2sym["scanf"] = SCANFSY;
    key2sym["printf"] = PRINTSY;
    key2sym["return"] = RETSY;
    sym2str[PLUSSY] = "PLUSSY";
    sym2str[MINUSSY] = "MINUSSY";
    sym2str[MULTSY] = "MULTSY";
    sym2str[DIVSY] = "DIVSY";
    sym2str[LSSY] = "LSSY";
    sym2str[LESY] = "LESY";
    sym2str[GTSY] = "GTSY";
    sym2str[GESY] = "GESY";
    sym2str[NEQSY] = "NEQSY";
    sym2str[EQUSY] = "EQUSY";
    sym2str[ASSIGNSY] = "ASSIGNSY";
    sym2str[LPARSY] = "LPAESY";
    sym2str[RPARSY] = "RPARSY";
    sym2str[LBRACKSY] = "LBRACKSY";
    sym2str[RBRACKSY] = "RBRACKSY";
    sym2str[LPRTSY] = "LPRTSY";
    sym2str[RPRTSY] = "RPRTSY";
    sym2str[COLONSY] = "COLONSY";
    sym2str[COMMASY] = "COMMASY";
    sym2str[SEMISY] = "SEMISY";
    sym2str[SQUOSY] = "SQUOSY";
    sym2str[DQUOSY] = "DQUOSY";
    sym2str[INTSY] = "INTSY";
    sym2str[CHARSY] = "CHARSY";
    sym2str[CONSTSY] = "CONSTSY";
    sym2str[VOIDSY] = "VOIDSY";
    sym2str[MAINSY] = "MAINSY";
    sym2str[SWITCHSY] = "SWITCHSY";
    sym2str[CASESY] = "CASESY";
    sym2str[DEFAULTSY] = "DEFAULTSY";
    sym2str[IFSY] = "IFSY";
    sym2str[LOOPSY] = "LOOPSY";
    sym2str[SCANFSY] = "SCANFSY";
    sym2str[PRINTSY] = "PRINTSY";
    sym2str[RETSY] = "RETSY";
    sym2str[INTEGERSY] = "INTEGERSY";
    sym2str[CHARACTERSY] = "CHARACTERSY";
    sym2str[STRINGSY] = "STRINGSY";
    sym2str[IDENTSY] = "IDENTSY";
}

char getchr(){
    char ch;
    if (cc == ll) {
        cnt ++;
        if (feof(fin)){
            fclose(fin);
            cc = 0;
            return EOF;
        }
        cc = ll = 0;
        do {
            if (fscanf(fin, "%c", &ch) != EOF) {
                line[ll++] = char(tolower(ch));
            } else {
                break;
            }
        } while (ch != '\n');
        line[ll] = '\0';
        cout << line << endl;
    }
    ch = line[cc ++];
    return ch;
}

symbol isReserved(char* word){
    for (auto &w: reserved_word)
        if (!strcmp(w, word)) return key2sym[word];
    return IDENTSY;
}

int transNum(char* word){
    int ret = 0;
    for (int i = 0; word[i] != '\0'; i ++){
        ret = (ret * 10 + word[i] - '0');
    }
    return ret;
}

void error(){
    fprintf(fout, "ERROR occurs at line %d columns %d\n", cnt, cc);
    cout << "exit with code -1" << endl;
}

void print(char *str, symbol sym){
    auto *sy = (char *)(sym2str[sym]).data();
    fprintf(fout, "SYMBOL: %s, CODE: %d, VALUE: \"%s\".\n", sy, sym, str);
}

void getsymInit(){
    memset(token, 0, sizeof(token));
    l = 0;
}

int getsym(){
    char ch;
    getsymInit();
    do ch = getchr(); while(isspace(ch));
    if (isalpha(ch)){
        while(isalpha(ch) || isdigit(ch)){
            token[l ++] = ch;
            ch = getchr();
        }
        result = isReserved(token);
        cc --;
    }
    else if (isdigit(ch)) {
        while (isdigit(ch)) {
            token[l++] = ch;
            ch = getchr();
        }
        result = INTEGERSY;
        cc --;
        num = transNum(token);
        printf("The decimal number is %d\n", num);
    }
    else if (ch == '+'){ token[l ++] = ch; result = PLUSSY;}
    else if (ch == '-'){ token[l ++] = ch; result = MINUSSY;}
    else if (ch == '*'){ token[l ++] = ch; result = MULTSY;}
    else if (ch == '/'){ token[l ++] = ch; result = DIVSY;}
    else if (ch == '('){ token[l ++] = ch; result = LPARSY;}
    else if (ch == ')'){ token[l ++] = ch; result = RPARSY;}
    else if (ch == '['){ token[l ++] = ch; result = LBRACKSY;}
    else if (ch == ']'){ token[l ++] = ch; result = RBRACKSY;}
    else if (ch == '{'){ token[l ++] = ch; result = LPRTSY;}
    else if (ch == '}'){ token[l ++] = ch; result = RPRTSY;}
    else if (ch == ','){ token[l ++] = ch; result = COMMASY;}
    else if (ch == ';'){ token[l ++] = ch; result = SEMISY;}
    else if (ch == ':'){ token[l ++] = ch; result = COLONSY;}
    else if (ch == '='){
        token[l ++] = ch;
        ch = getchr();
        if (ch == '='){
            token[l ++] = ch;
            result = EQUSY;
        }
        else{
            result = ASSIGNSY;
            cc --;
        }
    }
    else if (ch == '\''){
        token[l] = ch;
        result = SQUOSY;
        print(token, result);
        cout << token << endl;
        getsymInit();

        ch = getchr();
        token[l] = ch;
        result = CHARACTERSY;
        print(token, result);
        cout << token << endl;
        getsymInit();

        ch = getchr();
        if (ch == '\''){
            token[l ++] = ch;
            result = SQUOSY;
        }
        else{
            error();
            return -1;
        }
    }
    else if (ch == '\"'){
        token[l] = ch;
        result = DQUOSY;

        print(token, result);
        cout << token << endl;
        getsymInit();
        do {
            ch = getchr();
            token[l ++] = ch;
        }while (cc == ll || ch != '\"');
        if (ch == '\"'){
            token[l - 1] = '\0';
            result = STRINGSY;
            print(token, result);
            cout << token << endl;
            getsymInit();
            token[0] = '\"';
            token[1] = '\0';
            result = DQUOSY;
        }
        else{
            error();
            return -1;
        }
    }
    else if (ch == '!'){
        token[l ++] = ch;
        ch = getchr();
        if (ch == '=') {
            token[l ++] = ch;
            result = NEQSY;
        }
        else{
            error();
            return -1;
        }
    }
    else if (ch == '<'){
        token[l ++] = ch;
        ch = getchr();
        if (ch == '=') {
            token[l ++] = ch;
            result = LESY;
        }
        else {
            result = LSSY;
            cc --;
        }
    }
    else if (ch == '>'){
        token[l ++] = ch;
        ch = getchr();
        if (ch == '='){
            token[l ++] = ch;
            result = GESY;
        }
        else {
            result = GTSY;
            cc --;
        }
    }
    else if (ch == EOF || ch == '\0'){
        fclose(fout);
        return 0;
    }
    else {
        error();
        return -1;
    }
    token[l] = '\0';
    print(token, result);
    cout << token << endl;
    return 1;
}