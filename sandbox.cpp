#include "BigInt.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
    BigInt a("1001");
    BigInt b("2");

    if (a.is_negative() || b.is_negative()) {
        cout << "one of a or b is negative" << endl;
    }

    a -= b;

    cout << a << endl;
}
