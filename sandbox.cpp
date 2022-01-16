#include "BigInt.h"
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

int main() {
    BigInt a("1001");
    BigInt b("2");

    cout << "a - b: " << a - b << endl;
    cout << "b - a: " << b - a << endl;
    cout << "a - a: " << a - a << endl;
    cout << "b - b: " << b - b << endl;
}
