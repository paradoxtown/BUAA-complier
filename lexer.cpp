//
// Created by MUSE on 2018/11/13.
//
#include <iostream>
#include <cstring>
#include <cstdio>
#include <set>
#include <map>
#include "lexer.h"
#include "main.h"

using namespace std;
char line[ROWLENGTH][LENGTH];
int num = 0, rows = 0;
int cc = 0, ll = 0, l = 0, cnt = 0;
FILE *fin, *foutput;
//symbol result;
int symnum = 0;
map<string, symbol> key2sym;
map<symbol, string> sym2str;
syminfo syminfolist[SYMNUM + 1];

void init(char *path){
    symnum = 0;
    fin = fopen(path, "r");
    foutput = fopen("./result.txt", "w");
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

void readcode(){
    while(true) {
        fgets(line[rows], sizeof(line[rows]), fin);
        if (line[rows][0] == '\0' || feof(fin)) {
            for (ll = 0; line[rows][ll] != '\0'; ll ++);
            if (ll > 0) rows ++;// the last line has sentence
            fclose(fin);
            cc = 0;
            break;
        }
        for (ll = 0; line[rows][ll] != '\n'; ll ++);
        if (ll == 0) line[rows][ll ++] = ' ';
        line[rows][ll] = '\0'; // 0 -> end
        rows ++;
    }
    ll = 0;
}

char getchr(){
    char ch;
    if (cc == ll) {
        if (cnt == rows){
            return EOF;
        }
        cnt ++;
        cc = 0;
        for (ll = 0; line[cnt - 1][ll] != '\0'; ll ++);
    }
    ch = line[cnt - 1][cc ++];
    // cout << ch << endl;
    if (line[cnt - 1][cc - 2] == '\'' && line[cnt - 1][cc] == '\'') return ch;
    return char(tolower(ch));
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

void lexerror(){
    fprintf(foutput, "ERROR occurs at line %d columns %d\n", cnt, cc);
    cout << "exit with code -1" << endl;
}

void print(char *str, symbol sym){
    auto *sy = (char *)(sym2str[sym]).data();
    fprintf(foutput, "%3d, %12s, No.%3d : %s.\n", cnt, sy, sym, str);
}

void getsymInit(){
    memset(token, 0, sizeof(token));
    l = 0;
}

symbol returnsym(){
    cout << "am i wrong ?" << endl;
    return result;
}

char *returnname(){
    return token;
}

void back(int symnumber){
    symnum --; // because we also need to retract the symnum for symbol in correct position
    int index = syminfolist[symnumber % SYMNUM].prev;
    cnt = syminfolist[index].rownum;
    cc = syminfolist[index].columnnum;
}

int getsym(){
    char ch;
    getsymInit();
    do ch = getchr(); while(isspace(ch));
    if (isalpha(ch) || ch == '_'){
        while(isalpha(ch) || isdigit(ch) || ch == '_'){
            token[l ++] = ch;
            ch = getchr();
        }
        result = isReserved(token);
        cc --;
    }
    else if (isdigit(ch)) {
        if (ch != '0') {
            while (isdigit(ch)) {
                token[l++] = ch;
                ch = getchr();
            }
            num = transNum(token);
            result = INTEGERSY;
            cc--;
            // printf("The decimal number is %d\n", num);
        }
        else {
            ch = getchr();
            if (isdigit(ch)) {
                lexerror();
                return - 1;
            }
            else {
                num = 0;
                result = INTEGERSY;
                cc --;
            }
        }
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
        token[l ++] = ch;
        result = SQUOSY;
 //       setinfo();
//        print(token, result);
//        getsymInit();

        ch = getchr();
        token[l ++ ] = ch;
        result = CHARACTERSY;
 //       setinfo();
//        print(token[l - 1], result);
//        getsymInit();
        ch = getchr();
        if (ch == '\''){
            token[l ++] = ch;
            result = SQUOSY;
            // setinfo();
        }
        else{
            lexerror();
            return -1;
        }
    }
    else if (ch == '\"'){
        token[l] = ch;
        result = DQUOSY;
        // setinfo();
        print(token, result);
        // cout << token << endl;
        getsymInit();
        token[l ++] = '\"';
        do {
            ch = getchr();
            token[l ++] = ch;
        }while (cc == ll || ch != '\"');
        if (ch == '\"'){
            token[l] = '\0';
            result = STRINGSY;
            // setinfo();
            print(token, result);
            // cout << token << endl;
            // getsymInit();
            // token[l ++] = '\"';
            result = DQUOSY;
        }
        else{
            lexerror();
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
            lexerror();
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
        fclose(foutput);
        return 0;
    }
    else {
        lexerror();
        return -1;
    }
    token[l] = '\0';
    setinfo();
    print(token, result);
    // cout << token << endl;
    return 1;
}

void setinfo(){
    int index = symnum % SYMNUM;
    syminfolist[index].sym = result;
    strcpy(syminfolist[index].name, token);
    syminfolist[index].rownum = cnt;
    syminfolist[index].columnnum = cc; // the end column of this symbol
    syminfolist[index].prev = symnum == 0 ? 0 : ((symnum - 1) % SYMNUM);
    symnum ++;
}
