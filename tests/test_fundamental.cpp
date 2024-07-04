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

void test_floatpoint() {
    struct floatpoint_struct {
        float  f;
        double d;
    };
    auto fs = floatpoint_struct{
        100,
        200,
    };
    auto buf = fzto::serialize<std::string>(fs);
    auto fs2 = fzto::deserialize<floatpoint_struct>(buf);
    ASSERT(fs2.f - 100 < 0.001);
    ASSERT(fs2.d - 200 < 0.001);
}

void test_boolean() {
    struct boolean_struct {
        bool b1;
        bool b2;
    };
    auto bs = boolean_struct{
        true,
        false,
    };
    auto buf = fzto::serialize<std::string>(bs);
    auto bs2 = fzto::deserialize<boolean_struct>(buf);
    ASSERT(bs2.b1 == true);
    ASSERT(bs2.b2 == false);
}

void test_enum() {
    enum class Color {
        RED,
        GREEN,
        BLUE,
    };
    enum Status {
        INIT,
        RUNNING,
        EXIT,
    };
    struct enum_struct {
        Color  color;
        Status status;
    };

    auto es = enum_struct{
        Color::GREEN,
        RUNNING,
    };
    auto buf = fzto::serialize<std::string>(es);
    auto es2 = fzto::deserialize<enum_struct>(buf);
    ASSERT(es2.color == Color::GREEN);
    ASSERT(es2.status == RUNNING);
}

auto main() -> int {
    test_signed();
    test_unsigned();
    test_floatpoint();
    test_boolean();
    test_enum();
}
