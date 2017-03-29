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

#ifndef CPPFFI_SUPPORT_H
#define CPPFFI_SUPPORT_H

#include <cassert>
#include <memory>
#include <string>
#include <stdexcept>
#include <utility>

#include "cppffi_begin.h"

#if __cplusplus >= 201402L  // Is C++14 or newer
#define CPPFFI_CONSTEXPR_ASSERT(e) assert(e)
#elif __cplusplus >= 201103L
/*#define CPPFFI_CONSTEXPR_ASSERT(e)      \
    do {                                \
        if (!(e)) {                     \
            throw std::logic_error(#e); \
        }                               \
    } while (false)*/
#define CPPFFI_CONSTEXPR_ASSERT(e)
#endif

#if __cplusplus > 201103L  // Newer than C++11
#define CPPFFI_DEFAULT_COPY_CONSTRUCT_ASSIGN(class) \
    class(const class&) noexcept = default;         \
    class& operator=(const class&) noexcept = default;
#define CPPFFI_DEFAULT_CONSTEXPR_COPY_CONSTRUCT_ASSIGN(class) \
    constexpr class(const class&) noexcept = default;         \
    constexpr class& operator=(const class&) noexcept = default;
#define CPPFFI_DEFAULT_MOVE_CONSTRUCT_ASSIGN(class) \
    class(class &&) noexcept = default;             \
    class& operator=(class&&) noexcept = default;
#define CPPFFI_DEFAULT_CONSTEXPR_MOVE_CONSTRUCT_ASSIGN(class) \
    constexpr class(class &&) noexcept = default;             \
    constexpr class& operator=(class&&) noexcept = default;

#define CPPFFI_DELETE_COPY_CONSTRUCT_ASSIGN(class) \
    class(const class&) noexcept = delete;         \
    class& operator=(const class&) noexcept = delete;
#define CPPFFI_DELETE_CONSTEXPR_COPY_CONSTRUCT_ASSIGN(class) \
    constexpr class(const class&) noexcept = delete;         \
    constexpr class& operator=(const class&) noexcept = delete;
#define CPPFFI_DELETE_MOVE_CONSTRUCT_ASSIGN(class) \
    class(class &&) noexcept = delete;             \
    class& operator=(class&&) noexcept = delete;
#define CPPFFI_DELETE_CONSTEXPR_MOVE_CONSTRUCT_ASSIGN(class) \
    constexpr class(class &&) noexcept = delete;             \
    constexpr class& operator=(class&&) noexcept = delete;

#else

#define CPPFFI_DEFAULT_COPY_CONSTRUCT_ASSIGN(class) \
    class(const class&) noexcept = default;         \
    class& operator=(const class&) noexcept = default;
#define CPPFFI_DEFAULT_CONSTEXPR_COPY_CONSTRUCT_ASSIGN(class) \
    class(const class&) noexcept = default;                   \
    class& operator=(const class&) noexcept = default;
#define CPPFFI_DEFAULT_MOVE_CONSTRUCT_ASSIGN(class) \
    class(class &&) noexcept = default;             \
    class& operator=(class&&) noexcept = default;
#define CPPFFI_DEFAULT_CONSTEXPR_MOVE_CONSTRUCT_ASSIGN(class) \
    class(class &&) noexcept = default;                       \
    class& operator=(class&&) noexcept = default;

#define CPPFFI_DELETE_COPY_CONSTRUCT_ASSIGN(class) \
    class(const class&) noexcept = delete;         \
    class& operator=(const class&) noexcept = delete;
#define CPPFFI_DELETE_CONSTEXPR_COPY_CONSTRUCT_ASSIGN(class) \
    class(const class&) noexcept = delete;                   \
    class& operator=(const class&) noexcept = delete;
#define CPPFFI_DELETE_MOVE_CONSTRUCT_ASSIGN(class) \
    class(class &&) noexcept = delete;             \
    class& operator=(class&&) noexcept = delete;
#define CPPFFI_DELETE_CONSTEXPR_MOVE_CONSTRUCT_ASSIGN(class) \
    class(class &&) noexcept = delete;                       \
    class& operator=(class&&) noexcept = delete;
#endif  // __cplusplus > 201103L

namespace ffi {
    template <typename CharT, typename Traits = std::char_traits<CharT>>
    class basic_string_view {
    public:
        using traits_type = Traits;
        using value_type = CharT;
        using pointer = const CharT*;
        using const_pointer = const CharT*;
        using reference = const CharT&;
        using const_reference = const CharT&;
        using const_iterator = const CharT&;
        using iterator = const_iterator;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using reverse_iterator = const_reverse_iterator;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;
        static constexpr size_type npos = size_type(-1);

        constexpr basic_string_view() noexcept = default;

        template <typename Allocator>
        basic_string_view(
            const std::basic_string<CharT, Traits, Allocator>& str) noexcept
            : m_len{str.length()}, m_str{str.data()}
        {
        }

        constexpr basic_string_view(const_pointer str)
            : m_len{str == nullptr ? 0 : traits_type::length(str)}, m_str{str}
        {
        }
        constexpr basic_string_view(const_pointer str, size_type len)
            : m_len{len}, m_str{str}
        {
        }

        CPPFFI_DEFAULT_CONSTEXPR_COPY_CONSTRUCT_ASSIGN(basic_string_view)
        CPPFFI_DEFAULT_CONSTEXPR_MOVE_CONSTRUCT_ASSIGN(basic_string_view)

        ~basic_string_view() noexcept = default;

        constexpr const_iterator begin() const noexcept
        {
            return m_str;
        }
        constexpr const_iterator end() const noexcept
        {
            return m_str + m_len;
        }

        constexpr const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }
        constexpr const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        constexpr size_type size() const noexcept
        {
            return m_len;
        }
        constexpr size_type length() const noexcept
        {
            return m_len;
        }

        constexpr size_type max_size() const noexcept
        {
            return (npos - sizeof(size_type) - sizeof(void*)) /
                   sizeof(value_type) / 4;
        }
        constexpr bool empty() const noexcept
        {
            return m_len == 0;
        }

        constexpr const_reference operator[](size_type pos) const
        {
            CPPFFI_CONSTEXPR_ASSERT(pos < m_len);
            return *(m_str + pos);
        }
        constexpr const_reference at(size_type pos) const
        {
            return pos < m_len
                       ? *(m_str + pos)
                       : (throw std::logic_error("ffi::basic_string_view::at: "
                                                 "pos >= this->size()"),
                          *m_str);
        }

        constexpr const_reference front() const
        {
            CPPFFI_CONSTEXPR_ASSERT(m_len > 0);
            return *m_str;
        }
        constexpr const_reference back() const
        {
            CPPFFI_CONSTEXPR_ASSERT(m_len > 0);
            return *(m_str + m_len - 1);
        }

        constexpr pointer data() const noexcept
        {
            return m_str;
        }

        void swap(basic_string_view& sv) noexcept
        {
            std::swap(m_len, sv.m_len);
            std::swap(m_str, sv.m_str);
        }

        template <typename Allocator>
        explicit operator std::basic_string<CharT, Traits, Allocator>() const
        {
            return {m_str, m_len};
        }

        template <typename Allocator = std::allocator<CharT>>
        std::basic_string<CharT, Traits, Allocator> to_string(
            const Allocator& alloc = Allocator()) const
        {
            return {m_str, m_len, alloc};
        }

    private:
        size_type m_len{0};
        pointer m_str{nullptr};
    };
}  // namespace ffi

#if defined(__has_include) && __cplusplus > 201402L  // Is C++17
#if __has_include(<string_view>)
#include <string_view>
namespace ffi {
    using string_view = std::string_view;
}
// clang-format off
#elif __has_include(<experimental/string_view>)
// clang-format on
#include <experimental/string_view>
namespace ffi {
    using string_view = std::experimental::string_view;
}
#else
namespace ffi {
    using string_view = basic_string_view<char>;
}
#endif  // __has_include(<string_view>)
#else
namespace ffi {
    using string_view = basic_string_view<char>;
}
#endif  // defined(__has_include) && __cplusplus > 201402L

namespace ffi {
    class exception : public std::exception {
    };
    class bad_typedef : public exception {
    public:
        const char* what() const noexcept override
        {
            return "Bad typedef";
        }
    };
    class bad_abi : public exception {
    public:
        const char* what() const noexcept override
        {
            return "Bad ABI";
        }
    };
    class bad_stack_array_access : public exception {
    public:
        const char* what() const noexcept override
        {
            return "Bad stack_array access";
        }
    };
}  // namespace ffi

namespace ffi {
#if __cplusplus > 201402L  // Is C++17
    template <typename T>
    using is_nothrow_swappable = std::is_nothrow_swappable<T>;
#else
    // From P0185R1
    namespace swappable {

        template <class T>
        struct is_swappable;

        template <class T>
        struct is_nothrow_swappable;

        template <class T, class U>
        struct is_swappable_with;

        template <class T, class U>
        struct is_nothrow_swappable_with;

        template <class T>
        inline typename std::enable_if<std::is_move_constructible<T>::value &&
                                       std::is_move_assignable<T>::value>::type
        swap(T& a,
             T& b) noexcept(std::is_nothrow_move_constructible<T>::value&&
                                std::is_nothrow_move_assignable<T>::value);

        template <class T, std::size_t N>
        inline typename std::enable_if<is_swappable<T>::value>::type swap(
            T (&a)[N],
            T (&b)[N]) noexcept(is_nothrow_swappable<T>::value);

        namespace swappable_details {

            using swappable::swap;

            struct do_is_swappable {
                template <class T,
                          class = decltype(swap(std::declval<T&>(),
                                                std::declval<T&>()))>
                static std::true_type test(int);

                template <class>
                static std::false_type test(...);
            };

            struct do_is_nothrow_swappable {
                template <class T>
                static auto test(int) -> std::integral_constant<
                    bool,
                    noexcept(swap(std::declval<T&>(), std::declval<T&>()))>;

                template <class>
                static std::false_type test(...);
            };

            struct do_is_swappable_with {
                template <class T,
                          class U,
                          class = decltype(swap(std::declval<T>(),
                                                std::declval<U>())),
                          class = decltype(swap(std::declval<U>(),
                                                std::declval<T>()))>
                static std::true_type test(int);

                template <class, class>
                static std::false_type test(...);
            };

            struct do_is_nothrow_swappable_with {
                template <class T, class U>
                static auto test(int) -> std::integral_constant<
                    bool,
                    noexcept(swap(std::declval<T>(), std::declval<U>())) &&
                        noexcept(swap(std::declval<U>(), std::declval<T>()))>;

                template <class, class>
                static std::false_type test(...);
            };

            template <class T>
            struct is_swappable_impl : decltype(do_is_swappable::test<T>(0)) {
            };

            template <class T>
            struct is_nothrow_swappable_impl
                : decltype(do_is_nothrow_swappable::test<T>(0)) {
            };

            template <class T, class U>
            struct is_swappable_with_impl
                : decltype(do_is_swappable_with::test<T, U>(0)) {
            };

            // The following specialization is just a QoI optimization and
            // not actually required:
            template <class T>
            struct is_swappable_with_impl<T&, T&>
                : decltype(do_is_swappable::test<T&>(0)) {
            };

            template <class T, class U>
            struct is_nothrow_swappable_with_impl
                : decltype(do_is_nothrow_swappable_with::test<T, U>(0)) {
            };

            // The following specialization is just a QoI optimization and
            // not actually required:
            template <class T>
            struct is_nothrow_swappable_with_impl<T&, T&>
                : decltype(do_is_nothrow_swappable::test<T&>(0)) {
            };

        }  // namespace swappable_details

        template <class T>
        struct is_swappable : swappable_details::is_swappable_impl<T> {
        };

        template <class T>
        struct is_nothrow_swappable
            : swappable_details::is_nothrow_swappable_impl<T> {
        };

        template <class T, class U>
        struct is_swappable_with
            : swappable_details::is_swappable_with_impl<T, U> {
        };

        template <class T, class U>
        struct is_nothrow_swappable_with
            : swappable_details::is_nothrow_swappable_with_impl<T, U> {
        };

        template <class T>
        inline typename std::enable_if<std::is_move_constructible<T>::value &&
                                       std::is_move_assignable<T>::value>::type
        swap(T& a, T& b) noexcept(std::is_nothrow_move_constructible<T>::value&&
                                      std::is_nothrow_move_assignable<T>::value)
        {
            T tmp = std::move(a);
            a = std::move(b);
            b = std::move(tmp);
        }

        template <class T, std::size_t N>
        inline typename std::enable_if<is_swappable<T>::value>::type swap(
            T (&a)[N],
            T (&b)[N]) noexcept(is_nothrow_swappable<T>::value)
        {
            for (std::size_t i = 0; i != N; ++i) {
                using swappable::swap;
                swap(a[i], b[i]);
            }
        }
    }  // namespace swappable

    template <typename T>
    using is_nothrow_swappable = swappable::is_nothrow_swappable<T>;
#endif
}  // namespace ffi

namespace ffi {
    template <typename Element, size_t Size>
    struct stack_array {
        using value_type = Element;
        using pointer = Element*;
        using const_pointer = const Element*;
        using reference = Element&;
        using const_reference = const Element&;
        using iterator = Element*;
        using const_iterator = const Element*;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;
        using size_type = size_t;
        using difference_type = std::ptrdiff_t;

        Element m_elems[Size];

        void swap(stack_array& other) noexcept(
            is_nothrow_swappable<Element>::value)
        {
            std::swap_ranges(begin(), end(), other.begin());
        }

        iterator begin() noexcept
        {
            return iterator(data());
        }
        const_iterator begin() const noexcept
        {
            return const_iterator(data());
        }
        iterator end() noexcept
        {
            return iterator(data() + Size);
        }
        const_iterator end() const noexcept
        {
            return const_iterator(data() + Size);
        }

        constexpr size_type size() const noexcept
        {
            return Size;
        }
        constexpr size_type max_size() const noexcept
        {
            return Size;
        }
        constexpr bool empty() const noexcept
        {
            return size() == 0;
        }

        reference operator[](size_type n) noexcept
        {
            CPPFFI_CONSTEXPR_ASSERT(n < Size);
            return m_elems[n];
        }
        constexpr const_reference operator[](size_type n) const noexcept
        {
            CPPFFI_CONSTEXPR_ASSERT(n < Size);
            return m_elems[n];
        }

        reference at(size_type n)
        {
            return n < Size ? m_elems[n]
                            : (throw bad_stack_array_access{}, m_elems[0]);
        }
        constexpr const_reference at(size_type n) const
        {
            return n < Size ? m_elems[n]
                            : (throw bad_stack_array_access{}, m_elems[0]);
        }

        reference front() noexcept
        {
            return *begin();
        }
        constexpr const_reference front() const noexcept
        {
            return *begin();
        }

        reference back() noexcept
        {
            return *(end() - 1);
        }
        constexpr const_reference back() const noexcept
        {
            return *(end() - 1);
        }

        pointer data() noexcept
        {
            return m_elems;
        }
        constexpr const_pointer data() const noexcept
        {
            return m_elems;
        }
    };
}  // namespace ffi

namespace ffi {
    template <typename... Args>
    struct variadic_typedef {
    };

    template <typename... Args>
    struct convert_tuple {
        typedef std::tuple<Args...> type;
    };

    template <typename... Args>
    struct convert_tuple<variadic_typedef<Args...>> {
        typedef typename convert_tuple<Args...>::type type;
    };

    template <std::size_t I = 0, typename FuncT, typename... Tp>
    inline typename std::enable_if<I == sizeof...(Tp), void>::type for_each(
        std::tuple<Tp...>&,
        FuncT)  // Unused arguments are given no names.
    {
    }

    template <std::size_t I = 0, typename FuncT, typename... Tp>
        inline typename std::enable_if <
        I<sizeof...(Tp), void>::type for_each(std::tuple<Tp...>& t, FuncT f)
    {
        f(std::get<I>(t));
        for_each<I + 1, FuncT, Tp...>(t, f);
    }

    using deleter_type = std::function<void(void*)>;
    using unique_void_ptr = std::unique_ptr<void, deleter_type>;

    template <typename T>
    void unique_void_deleter(const void* data)
    {
        const T* p = static_cast<const T*>(data);
        delete p;
    }
    template <typename T>
    unique_void_ptr unique_void(T* ptr)
    {
        return unique_void_ptr(ptr, &unique_void_deleter<T>);
    }
}  // namespace ffi

#include "cppffi_end.h"

#endif
