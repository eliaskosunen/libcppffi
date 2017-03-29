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

#ifndef CPPFFI_TYPES_STRUCTS_H
#define CPPFFI_TYPES_STRUCTS_H

#include "../cppffi_begin.h"

#include "ffi.h"
#include <array>
#include <type_traits>

namespace ffi {
    template <typename T>
    class struct_type_specialize;

    template <typename StructT>
    class struct_type_specialize<StructT> {
    public:
        static const ffi_type& create_ffitype()
        {
            static ffi_type t;
            t.size = t.alignment = 0;
            t.type = FFI_TYPE_STRUCT;

            static ffi_type* field = nullptr;
            t.elements = &field;

            return t;
        }
    };

    template <typename StructT, typename... FieldsT>
    class struct_type_specialize<StructT, FieldsT...> {
    public:
        static const ffi_type& create_ffitype()
        {
            static ffi_type t;
            t.size = t.alignment = 0;

            static std::array<ffi_type*, sizeof...(FieldsT) + 1> fields;
            _fill_fields<decltype(fields), 0>(fields);
            fields[sizeof...(FieldsT)] = nullptr;
            t.elements = &fields[0];

            return t;
        }

    private:
        template <typename Container, size_t Index, typename Field>
        static void _fill_fields(Container& arr)
        {
            arr[Index] = Field::ffitype();
        }

        template <typename Container,
                  size_t Index,
                  typename FirstField,
                  typename... Fields>
        static void _fill_fields(Container& arr)
        {
            arr[Index] = FirstField::ffitype();
            _fill_fields<Container, Index + 1, Fields...>(arr);
        }
    };
}  // namespace ffi

#include "../cppffi_end.h"

#endif
