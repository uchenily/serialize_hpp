#include "serialize_hpp/debug.hpp"
#include "serialize_hpp/serialize.hpp"

struct signed_struct {
    int8_t  i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
};

auto main() -> int {
    auto ss = signed_struct{
        100,
        200,
        300,
        400,
    };
    auto buf = fzto::serialize<std::string>(ss);
    auto ss2 = fzto::deserialize<signed_struct>(buf);
    ASSERT(ss2.i8 == 100);
    ASSERT(ss2.i16 == 200);
    ASSERT(ss2.i32 == 300);
    ASSERT(ss2.i64 == 400);
}
