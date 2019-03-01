//#include <stdlib.h>
//#include <stdio.h>
//#include <iostream>
#include "stack.hpp"

using namespace std;

int main()
{
    Stack a;
    a.SwapDbDump();
    a.Push(42 / 5);
    a.Dump();
    a.Push(69);
    a.Dump();
    a.Push(228.5);
    a.Dump();


    sttype b = 0;
    a.Pop(&b);
    a.Pop(&b);
    a.Dump();
    //a.data_[1] = -1;
    a.Pop(&b);
    a.Dump();

    // bad:
    a.Pop(&b);
    a.Dump();

    return 0;
}
