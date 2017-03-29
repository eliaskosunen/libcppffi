#include "include/cppffi/cppffi.h"

#include <iostream>
#include <cstdio>

static int func()
{
    return 0;
}
static int func2(int i)
{
    return i;
}

int main()
{
    /*ffi::cif<int()> i{};
    auto f = i.bind(func);
    int ret;
    f.call(ret);
    std::cout << ret << '\n';

    std::cout << ffi::call(func2, 1) << '\n';*/

    ffi::cif<decltype(func2)> i;
    auto f = i.bind(func2);
    std::cout << f.call(1).ret() << '\n';
    return 0;
}
