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

// NO HEADER GUARDS!

#include <exception>
#include "cppffi_config.h"

#if defined(_MSC_VER) && _MSC_VER < 1910
#pragma push_macro("contexpr")
#define constexpr /*constexpr*/

#if _MSC_VER <= 1800
#pragma push_macro("noexcept")
#define noexcept /*noexcept*/
#endif           // _MSC_VER <= 1800
#endif           // defined(_MSC_VER) && _MSC_VER < 1910

#if !defined(__EXCEPTIONS) || defined(CPPFFI_NOTHROW)
#define CPPFFI_THROW(t) std::terminate()
#else
#define CPPFFI_THROW(t) throw t
#endif
