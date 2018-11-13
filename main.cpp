#include <iostream>
#include "lexer.h"

#define MAXN 20
#define LENGTH 81

using namespace std;

int main() {
    string path;
    cin >> path;
    init((char*)path.c_str());
    while(getsym() > 0) {}
    return 0;
}