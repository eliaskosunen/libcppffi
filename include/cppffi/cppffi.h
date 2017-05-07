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

#ifndef CPPFFI_H
#define CPPFFI_H

#include "ffi.h"
#include <array>
#include <cassert>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>

#include "cppffi_begin.h"

#include "cppffi_support.h"
#include "types/builtin.h"

namespace ffi {
    using abi = ffi_abi;

    /**
     * Call InterFace.
     * Represents a function prototype that can be bound to multiple callable
     * objects
     */
    template <typename T>
    class cif;

    template <typename T>
    class callable;

    template <typename T>
    class call_context;

    template <typename ReturnT, typename... ArgsT>
    class cif<ReturnT(ArgsT...)> {
    public:
        friend class callable<ReturnT(ArgsT...)>;
        friend class call_context<ReturnT(ArgsT...)>;

        cif(abi p_abi = FFI_DEFAULT_ABI);

        /**
         * Bind a function to the interface and produce a callable object
         * \param c Function to bind to
         * \return  callable
         */
        template <typename Callable>
        callable<ReturnT(ArgsT...)> bind(Callable&& c);

    private:
        template <size_t Index>
        void _expand_argument_list();
        template <size_t Index, typename FirstArg, typename... Args>
        void _expand_argument_list();

        ffi_cif m_cif;
        std::array<ffi_type*, sizeof...(ArgsT)> argtypes;
    };

    template <typename ReturnT>
    class cif<ReturnT()> {
    public:
        friend class callable<ReturnT()>;
        friend class call_context<ReturnT()>;

        cif(abi p_abi = FFI_DEFAULT_ABI);

        /**
         * Bind a function to the interface and produce a callable object
         * \param c Function to bind to
         * \return  callable
         */
        template <typename CallableT>
        callable<ReturnT()> bind(CallableT&& c);

    private:
        ffi_cif m_cif;
    };

    template <typename ReturnT, typename... ArgsT>
    class callable<ReturnT(ArgsT...)> {
    public:
        friend class call_context<ReturnT(ArgsT...)>;

        using return_type = ReturnT;
        using callable_type = ReturnT(ArgsT...);

        callable(cif<ReturnT(ArgsT...)>& p_cif, callable_type& p_callable);

        template <typename... Args>
        call_context<ReturnT(ArgsT...)> call(Args&&... args) const;

        template <typename... Args>
        ReturnT operator()(Args&&... args) const;

    private:
        template <size_t Index, typename ArgPack, typename Arg>
        void _fill_arg_vec(ArgPack& vec, Arg arg) const;
        template <size_t Index,
                  typename ArgPack,
                  typename FirstArg,
                  typename... Args>
        void _fill_arg_vec(ArgPack& vec,
                           FirstArg first_arg,
                           Args... args) const;

        callable_type& m_callable;
        cif<callable_type>& m_cif;
    };

    template <typename ReturnT>
    class callable<ReturnT()> {
    public:
        friend class call_context<ReturnT()>;

        using return_type = ReturnT;
        using callable_type = ReturnT();

        callable(cif<ReturnT()>& p_cif, callable_type& p_callable);

        call_context<ReturnT()> call() const;

        ReturnT operator()() const;

    private:
        callable_type& m_callable;
        cif<ReturnT()>& m_cif;
    };

    namespace detail {
        template <typename ReturnT>
        struct call_return {
            using type = ReturnT;

            template <typename T>
            static type access(T&& val)
            {
                return std::forward<T>(val);
            }
        };

        template <>
        struct call_return<void> {
            using type = int;

            template <typename T>
            static type access(T&&)
            {
                return 0;
            }
        };
    }

    template <typename ReturnT, typename... ArgsT>
    class call_context<ReturnT(ArgsT...)> {
    public:
        call_context(const callable<ReturnT(ArgsT...)>& p_callable,
                     ArgsT&&... args);

        ReturnT ret() const;

        typename detail::call_return<ReturnT>::type& ret_ref();
        const typename detail::call_return<ReturnT>::type& ret_ref() const;

        typename detail::call_return<ReturnT>::type&& ret_move();

    private:
        template <size_t Index, typename Arg>
        void _expand_argument_list(Arg&& arg);
        template <size_t Index, typename FirstArg, typename... Args>
        void _expand_argument_list(FirstArg&& arg, Args&&... args);

        template <size_t Index = 0>
        typename std::enable_if<Index == sizeof...(ArgsT), void>::type
        _get_argument_addresses(std::array<void*, sizeof...(ArgsT)>&)
        {
        }

        template <size_t Index = 0>
            typename std::enable_if <
            Index<sizeof...(ArgsT), void>::type _get_argument_addresses(
                std::array<void*, sizeof...(ArgsT)>& vec)
        {
            vec[Index] = std::addressof(std::get<Index>(m_args));
            _get_argument_addresses<Index + 1>(vec);
        }

        std::tuple<ArgsT...> m_args;
        const callable<ReturnT(ArgsT...)>& m_callable;
        typename detail::call_return<ReturnT>::type m_return;
    };

    template <typename ReturnT>
    class call_context<ReturnT()> {
    public:
        call_context(const callable<ReturnT()>& p_callable);

        ReturnT ret() const;

        typename detail::call_return<ReturnT>::type& ret_ref();
        const typename detail::call_return<ReturnT>::type& ret_ref() const;

        typename detail::call_return<ReturnT>::type&& ret_move();

    private:
        const callable<ReturnT()>& m_callable;
        typename detail::call_return<ReturnT>::type m_return;
    };

    template <typename ReturnT, typename... ArgsT>
    ReturnT call(ReturnT (&func)(ArgsT...), ArgsT&&... args);
}  // namespace ffi

// Include the implementation header
#include "cppffi_impl.h"

#include "cppffi_end.h"

#endif  // CPPFFI_H
