#include "serialize_hpp/debug.hpp"
#include "serialize_hpp/serialize.hpp"

#include <array>

void test_vector() {
    struct vector_struct {
        std::vector<int> v1;
        std::vector<int> v2;
    };
    auto vs = vector_struct{
        {  1,   2,   3},
        {100, 200, 300},
    };
    auto buf = fzto::serialize<std::string>(vs);
    auto vs2 = fzto::deserialize<vector_struct>(buf);
    auto expected_vec1 = std::vector{1, 2, 3};
    auto expected_vec2 = std::vector{100, 200, 300};
    ASSERT(vs2.v1 == expected_vec1);
    ASSERT(vs2.v2 == expected_vec2);
}

void test_array() {
    struct array_struct {
        std::array<int, 3> l1;
        std::array<int, 3> l2;
    };
    auto as = array_struct{
        {  1,   2,   3},
        {100, 200, 300},
    };
    auto buf = fzto::serialize<std::string>(as);
    auto as2 = fzto::deserialize<array_struct>(buf);
    auto expected_array1 = std::array{1, 2, 3};
    auto expected_array2 = std::array{100, 200, 300};
    ASSERT(as2.l1 == expected_array1);
    ASSERT(as2.l2 == expected_array2);
}

auto main() -> int {
    test_vector();
    test_array();
}
