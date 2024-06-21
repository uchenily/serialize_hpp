#include "serialize_hpp/debug.hpp"
#include "serialize_hpp/serialize.hpp"

void test_signed() {
    struct signed_struct {
        int8_t  i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
    };
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

void test_unsigned() {
    struct unsigned_struct {
        uint8_t  u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
    };
    auto us = unsigned_struct{
        100,
        200,
        300,
        400,
    };
    auto buf = fzto::serialize<std::string>(us);
    auto us2 = fzto::deserialize<unsigned_struct>(buf);
    ASSERT(us2.u8 == 100);
    ASSERT(us2.u16 == 200);
    ASSERT(us2.u32 == 300);
    ASSERT(us2.u64 == 400);
}

auto main() -> int {
    test_signed();
    test_unsigned();
}
