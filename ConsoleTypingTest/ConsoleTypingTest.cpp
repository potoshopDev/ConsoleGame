// c++20 need
//
#include "ConsoleMain.h"


int main()
{
    console::ConsoleMain myConsole(100i16, 25i16, 12i16);

    if (!myConsole.initlize_console())
        return 1;

    myConsole.Start();

    return 0;
}

