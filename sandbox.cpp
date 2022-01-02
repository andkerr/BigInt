#include "BigInt.h"
#include <iostream>

using namespace std;

int main() {
    BigInt a("3");
    BigInt b;

    b = a += BigInt("2");

    cout << a << " " << b << endl;
}
