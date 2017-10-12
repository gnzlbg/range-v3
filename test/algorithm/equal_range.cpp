// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <vector>
#include <iterator>
#include <range/v3/core.hpp>
#include <range/v3/view/join.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/repeat_n.hpp>
#include <range/v3/view/take.hpp>
#include <range/v3/utility/iterator.hpp>
#include <range/v3/algorithm/copy.hpp>
#include <range/v3/algorithm/equal_range.hpp>
#include "../simple_test.hpp"
#include "../test_iterators.hpp"

struct my_int
{
    int value;
};

bool compare(my_int lhs, my_int rhs)
{
    return lhs.value < rhs.value;
}

void not_totally_ordered()
{
    // This better compile!
    std::vector<my_int> vec;
    ranges::equal_range(vec, my_int{10}, compare);
}

template<class Iter, class Sent, class T, class Proj = ranges::ident>
RANGES_CXX14_CONSTEXPR
bool
test(Iter first, Sent last, const T& value, Proj proj = Proj{})
{
    ranges::iterator_range<Iter, Iter> i =
        ranges::equal_range(first, last, value, ranges::ordered_less{}, proj);
    for (Iter j = first; j != i.begin(); ++j)
        if (!(ranges::invoke(proj, *j) < value)) { return false; }
    for (Iter j = i.begin(); j != last; ++j)
        if (ranges::invoke(proj, *j) < value) { return false; }
    for (Iter j = first; j != i.end(); ++j)
        if (value < ranges::invoke(proj, *j)) { return false; }
    for (Iter j = i.end(); j != last; ++j)
        if (!(value < ranges::invoke(proj, *j))) { return false; }

    auto res = ranges::equal_range(
        ranges::make_iterator_range(first, last), value, ranges::ordered_less{}, proj);
    for (Iter j = first; j != res.get_unsafe().begin(); ++j)
        if (!(ranges::invoke(proj, *j) < value)) { return false; }
    for (Iter j = res.get_unsafe().begin(); j != last; ++j)
        if (ranges::invoke(proj, *j) < value) { return false; }
    for (Iter j = first; j != res.get_unsafe().end(); ++j)
        if (value < ranges::invoke(proj, *j)) { return false; }
    for (Iter j = res.get_unsafe().end(); j != last; ++j)
        if (!(value < ranges::invoke(proj, *j))) { return false; }

    return true;
}

template<class Iter, class Sent = Iter>
RANGES_CXX14_CONSTEXPR
bool
test()
{
    using namespace ranges::view;
    constexpr unsigned N = 100;
    constexpr unsigned M = 10;
    int v[M * N]{-1};
    auto input = ints | take(N) | transform([](int i){return repeat_n(i,M);}) | join;
    ranges::copy(input, v);
    for (int x = 0; x <= (int)M; ++x)
        if (!test(Iter(v), Sent(v + M * N), x)) { return false; }

    return true;
}

RANGES_CXX14_CONSTEXPR
bool test_some() {
    int d[] = {0, 1, 2, 3};
    for (int e = 0; e <= 4; ++e)
        for (int x = -1; x <= 4; ++x)
          if (!test(d, &(d[e]), x)) { return false; }

    return true;
}

int main()
{
    CHECK(test_some());
#if RANGES_CXX_CONSTEXPR >= RANGES_CXX_CONSTEXPR_14
    static_assert(test_some(), "");
#endif

    CHECK(test<forward_iterator<const int*> >());
#if RANGES_CXX_CONSTEXPR >= RANGES_CXX_CONSTEXPR_14
    static_assert(test<forward_iterator<const int*> >(), "");
#endif
    CHECK(test<bidirectional_iterator<const int*> >());
    CHECK(test<random_access_iterator<const int*> >());
    CHECK(test<const int*>());

    CHECK(test<forward_iterator<const int*>, sentinel<const int*> >());
    CHECK(test<bidirectional_iterator<const int*>, sentinel<const int*> >());
    CHECK(test<random_access_iterator<const int*>, sentinel<const int*> >());

    {
        struct foo { int i; };

        foo some_foos[] = {{1}, {2}, {4}};
        CHECK(test(some_foos, some_foos + 3, 2, &foo::i));
    }

    return ::test_result();
}
