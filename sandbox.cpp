#include "BigInt.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    BigInt a("111");

    a += BigInt("999");

    if (a.is_negative()) {
        cout << "a is negative" << endl;
    }

    cout << a << endl;
}
