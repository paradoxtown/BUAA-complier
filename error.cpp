//
// Created by MUSE on 2018/11/17.
//

#include "error.h"
#include <map>
#include <fstream>
#define cout fout1

ofstream fout1("./error");
map<errorstate, string>sta2str;

void errorinit() {
    sta2str[HASBEENDEFINED] = "HASBEENDEFINED";
    sta2str[NOTBEENDEFINED] = "NOTBEENDEFINED";
    sta2str[ERRORNUM] = "ERRORNUM";
    sta2str[ERRORRETVALUE] = "ERRORRETVALUE";
    sta2str[NOTLPAR] = "NOTLPAR";
    sta2str[NOTRPAR] = "NOTRPAR";
    sta2str[NOTLPRT] = "NOTLPRT";
    sta2str[NOTRPRT] = "NOTRPRT";
    sta2str[NOTLBRA] = "NOTLBRA";
    sta2str[NOTRBRA] = "NOTRBRA";
    sta2str[NOTVOID] = "NOTVOID";
    sta2str[NOTMAIN] = "NOTMAIN";
    sta2str[NOTDQUO] = "NOTDQUO";
    sta2str[NOTSQUO] = "NOTSQUO";
    sta2str[NOTPLUS] = "NOTPLUS";
    sta2str[NOTASSIGN] = "NOTASSIGN";
    sta2str[NOTIDENT] = "NOTIDENT";
    sta2str[NOTTYP] = "NOTTYP";
    sta2str[NOTCONST] = "NOTCONST";
    sta2str[NOTSEMI] = "NOTSEMI";
    sta2str[NOTINTEGER] = "NOTINTEGER";
    sta2str[NOTCHAR] = "NOTCHAR";
    sta2str[NOMAIN] = "NOMAIN";
    sta2str[ERRORRET] = "ERRORRET";
    sta2str[ERRORRECORD] = "ERRORRECORD";
    sta2str[ERRORSTATEMENT] = "ERRORSTATEMENT";
    sta2str[NOTIF] = "NOTIF";
    sta2str[NOTCONDITION] = "NOTCONDITION";
    sta2str[NOTLOOP] = "NOTLOOP";
    sta2str[NOTSWITCH] = "NOTSWITCH";
    sta2str[NOTCASE] = "NOTCASE";
    sta2str[NOTDEFAULT] = "NOTDEFAULT";
    sta2str[NOTPRINT] = "NOTPRINT";
    sta2str[NOTSCAN] = "NOTSCAN";
    sta2str[NOTRET] = "NOTRET";
    sta2str[NOTCOLON] = "NOTCOLON";
    sta2str[OUTSTACK] = "OUTSTACK";
    sta2str[ERRORDEFAULT] = "ERRORDEFAULT";
}

void error(errorstate state, int line) {
    cout << sta2str[state] << " occurs at line : " << line << endl;
    iserror = true;
    return;
}

void error(){
    cout << "Some thing wrong." << endl;
    return;
};