#include "dummy.h"

using namespace std;

Dummy::Dummy(int x_, int y_) {
    x = x_;
    y = y_;
}

int Dummy::test() {
    cout << "Test done" << endl;
    return 0;
}