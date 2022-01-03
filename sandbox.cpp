#include "BigInt.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    string val = "125432";
    BigInt a(val);

    a = -a;

    if (a.is_negative()) {
        cout << "a is negative" << endl;
    }

    cout << a << endl;
}
