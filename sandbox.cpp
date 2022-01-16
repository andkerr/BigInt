#include "BigInt.h"
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

int main() {
    BigInt a("2");
    BigInt b("-2");

    cout << a + b << endl;

}
