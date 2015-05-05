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

//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <vector>
#include <range/v3/core.hpp>
#include <range/v3/algorithm/generate_n.hpp>
#include "../simple_test.hpp"
#include "../test_utils.hpp"
#include "../test_iterators.hpp"

struct gen_test
{
    int i_;
    RANGES_CXX14_CONSTEXPR gen_test() : i_{} {}
    RANGES_CXX14_CONSTEXPR gen_test(int i) : i_(i) {}
    RANGES_CXX14_CONSTEXPR int operator()() {return i_++;}
};

template <class Iter, class Sent = Iter>
void
test()
{
    const unsigned n = 4;
    int ia[n] = {0};
    std::pair<Iter, gen_test> res = ranges::generate_n(Iter(ia), n, gen_test(1));
    CHECK(ia[0] == 1);
    CHECK(ia[1] == 2);
    CHECK(ia[2] == 3);
    CHECK(ia[3] == 4);
    CHECK(res.first == Iter(ia + n));
    CHECK(res.second.i_ == 5);
}

void test2()
{
    // Test ranges::generate with a genuine output range
    std::vector<int> v;
    ranges::generate_n(ranges::back_inserter(v), 5, gen_test(1));
    CHECK(v.size() == 5u);
    CHECK(v[0] == 1);
    CHECK(v[1] == 2);
    CHECK(v[2] == 3);
    CHECK(v[3] == 4);
    CHECK(v[4] == 5);
}

#ifdef RANGES_CXX_GREATER_THAN_11
template<class Iter, class Sent = Iter>
RANGES_CXX14_CONSTEXPR bool test_constexpr()
{
    bool r = true;
    const unsigned n = 4;
    int ia[n] = {0};
    std::pair<Iter, gen_test> res(ranges::generate_n(Iter(ia), n, gen_test(1)));
    if(ia[0] != 1) { r = false; }
    if(ia[1] != 2) { r = false; }
    if(ia[2] != 3) { r = false; }
    if(ia[3] != 4) { r = false; }
    if(res.first != Iter(ia + n)) { r = false; }
    if(res.second.i_ != 5) { r = false; }
    return r;
}
#endif

int main()
{
    test<forward_iterator<int*> >();
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

    test<forward_iterator<int*>, sentinel<int*> >();
    test<bidirectional_iterator<int*>, sentinel<int*> >();
    test<random_access_iterator<int*>, sentinel<int*> >();

    test2();

#ifdef RANGES_CXX_GREATER_THAN_11
    {
        static_assert(test_constexpr<forward_iterator<int*> >(), "");
        static_assert(test_constexpr<bidirectional_iterator<int*> >(), "");
        static_assert(test_constexpr<random_access_iterator<int*> >(), "");
        static_assert(test_constexpr<int*>(), "");

        static_assert(test_constexpr<forward_iterator<int*>, sentinel<int*> >(), "");
        static_assert(test_constexpr<bidirectional_iterator<int*>, sentinel<int*> >(), "");
        static_assert(test_constexpr<random_access_iterator<int*>, sentinel<int*> >(), "");
    }
#endif

    return ::test_result();
}
