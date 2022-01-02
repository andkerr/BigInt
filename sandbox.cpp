#include "BigInt.h"
#include <iostream>

using namespace std;

int main() {
    BigInt my_int("333");
    BigInt my_int2("444");

    BigInt sum = my_int + my_int2;

    cout << sum << endl;
}
