/// \file
// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3

#ifndef RANGES_V3_UTILITY_ADDRESSOF_HPP
#define RANGES_V3_UTILITY_ADDRESSOF_HPP

#include <range/v3/detail/config.hpp>
#include <range/v3/utility/static_const.hpp>

#if RANGES_CXX_CONSTEXPR < RANGES_CXX_CONSTEXPR_17
#include <memory>
#endif

namespace ranges
{
    inline namespace v3
    {

#if RANGES_CXX_CONSTEXPR < RANGES_CXX_CONSTEXPR_17
        // This fallback code below is adapted from the Eggs.Variant
        // library which can be found here: https://github.com/eggs-cpp/variant
        //
        // Copyright Agustin K-ballo Berge, Fusion Fenix 2014-2015
        //
        // Distributed under the Boost Software License, Version 1.0. (See accompanying
        // file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

        namespace addressof_detail
        {

            struct fallback {};

            template <typename T>
            fallback operator&(T&&);

            template <typename T>
            struct has_addressof_operator
            {
                static constexpr bool value =
                    (std::is_class<T>::value || std::is_union<T>::value)
                     && !std::is_same<decltype(&std::declval<T&>()), fallback>::value;
            };
        }  //  namespace addressof_detail

        struct addressof_fn
        {
            template <typename T>
            constexpr typename std::enable_if<
                !addressof_detail::has_addressof_operator<T>::value, T* >::type
            operator()(T& r) const noexcept
            {
                return &r;
            }

            template <typename T>
            typename std::enable_if<
                addressof_detail::has_addressof_operator<T>::value, T*>::type
            operator()(T& r) const noexcept
            {
                return std::addressof(r);
            }
        };

#else  // RANGES_CXX_CONSTEXPR >= RANGES_CXX_CONSTEXPR_17

        struct addressof_fn
        {
            template <typename T>
            constexpr T* operator()(T& r) const noexcept
            {
                return std::addressof(r);
            }
        };

#endif  // RANGES_CXX_CONSTEXPR

        /// \ingroup group-utility
        /// \sa `addressof_fn`
        RANGES_INLINE_VARIABLE(addressof_fn, addressof)
    } // namespace v3
}  // namespace ranges

#endif
