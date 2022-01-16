#include "BigInt.h"
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

int main() {
    BigInt a("1001");
    BigInt b("2");

    cout << "-1 % 10: " << -1 % 10 << endl;
    cout << "-1 mod 10: " << -1 - 10 * std::floor(-1.0 / 10.0) << endl;

    cout << "a - b: " << a - b << endl;
    cout << "b - a: " << b - a << endl;
    cout << "a - a: " << a - a << endl;
    cout << "b - b: " << b - b << endl;
}
