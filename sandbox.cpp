#include "BigInt.h"
#include <cmath>
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
    cout << floor(-1.3) << endl;
}
