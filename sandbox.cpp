#include "BigInt.h"
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

int main() {
    BigInt a = 1;
    for (int i = 1; i <= 50; ++i) {
        a = a * BigInt(i);
    }
    cout << a << endl;
}
