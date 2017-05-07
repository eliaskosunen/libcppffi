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

#ifndef CPPFFI_IMPL_H
#define CPPFFI_IMPL_H

#include "ffi.h"
#include "cppffi.h"

#include "cppffi_begin.h"

namespace ffi {
    template <typename ReturnT, typename... ArgsT>
    inline cif<ReturnT(ArgsT...)>::cif(abi p_abi)
    {
        constexpr const size_t arg_count = sizeof...(ArgsT);
        _expand_argument_list<0, ArgsT...>();
        const auto status = ffi_prep_cif(
            &m_cif, p_abi, arg_count, &type<ReturnT>::ffitype(), &argtypes[0]);
        if (status == FFI_BAD_TYPEDEF) {
            CPPFFI_THROW(bad_typedef());
        }
        if (status == FFI_BAD_ABI) {
            CPPFFI_THROW(bad_abi());
        }
    }

    template <typename ReturnT, typename... ArgsT>
    template <typename Callable>
    inline callable<ReturnT(ArgsT...)> cif<ReturnT(ArgsT...)>::bind(
        Callable&& c)
    {
        return callable<ReturnT(ArgsT...)>(*this, c);
    }

    template <typename ReturnT, typename... ArgsT>
    template <size_t Index>
    inline void cif<ReturnT(ArgsT...)>::_expand_argument_list()
    {
    }
    template <typename ReturnT, typename... ArgsT>
    template <size_t Index, typename FirstArg, typename... Args>
    inline void cif<ReturnT(ArgsT...)>::_expand_argument_list()
    {
        argtypes[Index] = &type<FirstArg>::ffitype();
        _expand_argument_list<Index + 1, Args...>();
    }

    template <typename ReturnT>
    inline cif<ReturnT()>::cif(abi p_abi)
    {
        const auto status =
            ffi_prep_cif(&m_cif, p_abi, 0, &type<ReturnT>::ffitype(), nullptr);
        if (status == FFI_BAD_TYPEDEF) {
            CPPFFI_THROW(bad_typedef());
        }
        if (status == FFI_BAD_ABI) {
            CPPFFI_THROW(bad_abi());
        }
    }

    template <typename ReturnT>
    template <typename Callable>
    inline callable<ReturnT()> cif<ReturnT()>::bind(Callable&& c)
    {
        return callable<ReturnT()>(*this, c);
    }

    template <typename ReturnT, typename... ArgsT>
    inline callable<ReturnT(ArgsT...)>::callable(cif<ReturnT(ArgsT...)>& p_cif,
                                                 callable_type& p_callable)
        : m_callable(p_callable), m_cif(p_cif)
    {
    }

    template <size_t Index, typename Tuple>
    inline void* _get_tuple_elem_address(Tuple& t)
    {
        return static_cast<void*>(std::addressof(std::get<Index>(t)));
    }

    template <typename ReturnT, typename... ArgsT>
    template <typename... Args>
    inline call_context<ReturnT(ArgsT...)> callable<ReturnT(ArgsT...)>::call(
        Args&&... args) const
    {
        return call_context<ReturnT(ArgsT...)>{*this,
                                               std::forward<Args>(args)...};
    }

    template <typename ReturnT, typename... ArgsT>
    template <typename... Args>
    inline ReturnT callable<ReturnT(ArgsT...)>::operator()(Args&&... args) const
    {
        return call(std::forward<Args>(args)...).ret_move();
    }

    template <typename ReturnT, typename... ArgsT>
    template <size_t Index, typename ArgPack, typename Arg>
    inline void callable<ReturnT(ArgsT...)>::_fill_arg_vec(ArgPack& vec,
                                                           Arg arg) const
    {
        vec[Index] = static_cast<void*>(arg);
        return;
    }
    template <typename ReturnT, typename... ArgsT>
    template <size_t Index,
              typename ArgPack,
              typename FirstArg,
              typename... Args>
    inline void callable<ReturnT(ArgsT...)>::_fill_arg_vec(ArgPack& vec,
                                                           FirstArg first_arg,
                                                           Args... args) const
    {
        vec[Index] = static_cast<void*>(first_arg);
        _fill_arg_vec<Index + 1>(vec, std::addressof(args)...);
    }

    template <typename ReturnT>
    inline callable<ReturnT()>::callable(cif<ReturnT()>& p_cif,
                                         callable_type& p_callable)
        : m_callable(p_callable), m_cif(p_cif)
    {
    }

    template <typename ReturnT>
    inline call_context<ReturnT()> callable<ReturnT()>::call() const
    {
        return call_context<ReturnT()>{*this};
    }

    template <typename ReturnT>
    inline ReturnT callable<ReturnT()>::operator()() const
    {
        return call().ret_move();
    }

    template <typename ReturnT, typename... ArgsT>
    call_context<ReturnT(ArgsT...)>::call_context(
        const callable<ReturnT(ArgsT...)>& p_callable,
        ArgsT&&... args)
        : m_callable(p_callable)
    {
        _expand_argument_list<0>(std::forward<ArgsT>(args)...);

        std::array<void*, sizeof...(ArgsT)> arg_ptrs;
        _get_argument_addresses<0>(arg_ptrs);

        typename type<ReturnT>::arg_type ret;
        ffi_call(&p_callable.m_cif.m_cif, FFI_FN(p_callable.m_callable), &ret,
                 &arg_ptrs[0]);
        m_return = detail::call_return<ReturnT>::access(std::move(ret));
    }

    template <typename ReturnT, typename... ArgsT>
    ReturnT call_context<ReturnT(ArgsT...)>::ret() const
    {
        return static_cast<ReturnT>(m_return);
    }

    template <typename ReturnT, typename... ArgsT>
    typename detail::call_return<ReturnT>::type&
    call_context<ReturnT(ArgsT...)>::ret_ref()
    {
        return m_return;
    }
    template <typename ReturnT, typename... ArgsT>
    const typename detail::call_return<ReturnT>::type&
    call_context<ReturnT(ArgsT...)>::ret_ref() const
    {
        return m_return;
    }

    template <typename ReturnT, typename... ArgsT>
    typename detail::call_return<ReturnT>::type&&
    call_context<ReturnT(ArgsT...)>::ret_move()
    {
        return std::move(m_return);
    }

    template <typename ReturnT, typename... ArgsT>
    template <size_t Index, typename Arg>
    void call_context<ReturnT(ArgsT...)>::_expand_argument_list(Arg&& arg)
    {
        // m_args[Index] = unique_void(new Arg(arg));
        /* m_args.push_back(unique_void(new Arg(arg))); */
        std::get<Index>(m_args) = std::forward<Arg>(arg);
    }
    template <typename ReturnT, typename... ArgsT>
    template <size_t Index, typename FirstArg, typename... Args>
    void call_context<ReturnT(ArgsT...)>::_expand_argument_list(FirstArg&& arg,
                                                                Args&&... args)
    {
        // m_args[Index] = unique_void(new FirstArg(arg));
        /* m_args.push_back(unique_void(new FirstArg(arg))); */
        std::get<Index>(m_args) = std::forward<FirstArg>(arg);
        _expand_argument_list<Index + 1>(std::forward<Args>(args)...);
    }

    template <typename ReturnT>
    call_context<ReturnT()>::call_context(const callable<ReturnT()>& p_callable)
        : m_callable(p_callable)
    {
        typename type<ReturnT>::arg_type ret;
        ffi_call(&p_callable.m_cif.m_cif, FFI_FN(p_callable.m_callable), &ret,
                 nullptr);
        m_return = detail::call_return<ReturnT>::access(std::move(ret));
    }

    template <typename ReturnT>
    ReturnT call_context<ReturnT()>::ret() const
    {
        return static_cast<ReturnT>(m_return);
    }

    template <typename ReturnT>
    typename detail::call_return<ReturnT>::type&
    call_context<ReturnT()>::ret_ref()
    {
        return m_return;
    }
    template <typename ReturnT>
    const typename detail::call_return<ReturnT>::type&
    call_context<ReturnT()>::ret_ref() const
    {
        return m_return;
    }

    template <typename ReturnT>
    typename detail::call_return<ReturnT>::type&&
    call_context<ReturnT()>::ret_move()
    {
        return std::move(m_return);
    }

    template <typename ReturnT, typename... ArgsT>
    inline ReturnT call(ReturnT (&func)(ArgsT...), ArgsT&&... args)
    {
        cif<ReturnT(ArgsT...)> c;
        auto f = c.bind(func);
        return f.call(std::forward<ArgsT>(args)...).ret();
    }
}  // namespace ffi

#include "cppffi_end.h"

#endif
