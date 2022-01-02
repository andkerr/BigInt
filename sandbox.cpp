#include "BigInt.h"
#include <iostream>

using namespace std;

int main() {
    BigInt my_int("999");
    BigInt my_int2("999");

    BigInt sum = my_int + my_int2;

    cout << my_int << endl;
    cout << my_int2 << endl;
    cout << sum << endl;
}
