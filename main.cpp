#include <iostream>
#include "lexer.h"

using namespace std;

int main() {
    string path;
    cin >> path;
    init((char*)path.c_str());
    while(getsym() > 0) {}
    return 0;
}