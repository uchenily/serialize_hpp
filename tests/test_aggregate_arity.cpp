#include "serialize_hpp/aggregate_arity.hpp"
#include "serialize_hpp/debug.hpp"

auto main() -> int {
    using S = struct {
        int  i;
        char c;
        bool b;
    };
    S s{1, '2', true};

    auto &i = fzto::get<0>(s);
    auto &c = fzto::get<1>(s);
    auto &b = fzto::get<2>(s);
    static_assert(std::is_same<decltype(i), int &>{});
    static_assert(std::is_same<decltype(c), char &>{});
    static_assert(std::is_same<decltype(b), bool &>{});
    ASSERT(i == 1);
    ASSERT(c == '2');
    ASSERT(b == true);
    s.i = 2;
    s.c = '3';
    s.b = false;
    ASSERT(i == 2);
    ASSERT(c == '3');
    ASSERT(b == false);
}
