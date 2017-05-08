# libcppffi

[![Build Status](https://travis-ci.org/eliaskosunen/libcppffi.svg?branch=master)](https://travis-ci.org/eliaskosunen/libcppffi)
[![Coverage Status](https://coveralls.io/repos/github/eliaskosunen/libcppffi/badge.svg?branch=master)](https://coveralls.io/github/eliaskosunen/libcppffi?branch=master)

Header-only templated C++ wrapper over the C library [libffi](https://github.com/libffi/libffi)

WORK IN PROGRESS. It's buggy and lacks features. There isn't any documentation yet.

Essentially a zero-cost abstraction that gives you strong type-safety.

## Example

```cpp
#include <cppffi.h>
#include <iostream>

int foo()
{
    return 42;
}

void bar(float f)
{
    std::cout << f << '\n';
}

int main()
{
    std::cout << ffi::call(foo) << '\n';
    ffi::call(bar, 3.14f);

    ffi::cif<int()> foo_cif;
    auto foo_call = foo_cif.bind(foo);
    std::cout << foo_call() << '\n';
}
```

## License

libcppffi is licensed under the MIT license.

Copyright 2017 Elias Kosunen

