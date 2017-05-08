// Copyright 2017 Elias Kosunen
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest.h>
#include <cppffi.h>
#include <type_traits>

static int func(bool reset = false)
{
    static int counter = 0;
    if (reset) {
        return counter = 0;
    }
    return counter++;
}

static int factorial(int n)
{
    return n > 1 ? factorial(n - 1) * n : 1;
}

TEST_CASE("Test doctest: factorial")
{
    CHECK(factorial(0) == 1);
    CHECK(factorial(1) == 1);
    CHECK(factorial(2) == 2);
    CHECK(factorial(3) == 6);
    CHECK(factorial(10) == 3628800);
}

TEST_CASE("ffi::call")
{
    SUBCASE("No parameters")
    {
        CHECK(ffi::call(func, false) == 0);
        CHECK(ffi::call(func, false) == 1);
        CHECK(ffi::call(func, false) == 2);
        CHECK(ffi::call(func, true) == 0);
    }

    SUBCASE("1 parameter")
    {
        CHECK(ffi::call(factorial, 0) == 1);
        CHECK(ffi::call(factorial, 1) == 1);
        CHECK(ffi::call(factorial, 2) == 2);
        CHECK(ffi::call(factorial, 3) == 6);
        CHECK(ffi::call(factorial, 10) == 3628800);
    }
}
