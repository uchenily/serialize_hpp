#include "serialize_hpp/debug.hpp"
#include "serialize_hpp/serialize.hpp"

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
auto main() -> int {
    test_vector();
}
