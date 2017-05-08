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

#ifndef CPPFFI_TYPES_BUILTIN_H
#define CPPFFI_TYPES_BUILTIN_H

#include "ffi.h"
#include <cstdint>
#include <type_traits>

#include "../cppffi_begin.h"

namespace ffi {
    template <typename T, typename Enable = void>
    struct type;

    template <>
    struct type<void> {
        using arg_type = ffi_arg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_void;
        }
    };

    template <>
    struct type<bool> {
        using arg_type = ffi_arg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_uint8;
        }
    };

    template <>
    struct type<uint8_t> {
        using arg_type = ffi_arg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_uint8;
        }
    };
    template <>
    struct type<uint16_t> {
        using arg_type = ffi_arg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_uint16;
        }
    };
    template <>
    struct type<uint32_t> {
        using arg_type = ffi_arg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_uint32;
        }
    };
    template <>
    struct type<uint64_t> {
        using arg_type = ffi_arg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_uint64;
        }
    };

    template <>
    struct type<int8_t> {
        using arg_type = ffi_sarg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_sint8;
        }
    };
    template <>
    struct type<int16_t> {
        using arg_type = ffi_sarg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_sint16;
        }
    };
    template <>
    struct type<int32_t> {
        using arg_type = ffi_sarg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_sint32;
        }
    };
    template <>
    struct type<int64_t> {
        using arg_type = ffi_sarg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_sint64;
        }
    };

    template <>
    struct type<float> {
        using arg_type = ffi_arg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_float;
        }
    };
    template <>
    struct type<double> {
        using arg_type = ffi_arg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_double;
        }
    };
    template <>
    struct type<long double> {
        using arg_type = ffi_arg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_longdouble;
        }
    };

    template <typename T>
    struct type<T,
                typename std::enable_if<std::is_pointer<
                    typename std::decay<T>::type>::value>::type> {
        using arg_type = ffi_arg;

        static constexpr ffi_type& ffitype()
        {
            return ffi_type_pointer;
        }
    };

    template <typename T>
    struct type<T, typename std::enable_if<!std::is_pointer<
        typename std::decay<T>::type>::value>::type> {
        using arg_type = ffi_arg;

        static const ffi_type& ffitype()
        {
            return T::create_ffitype();
        }
    };
}  // namespace ffi

#include "../cppffi_end.h"

#endif
