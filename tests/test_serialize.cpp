#include "serialize_hpp/debug.hpp"
#include "serialize_hpp/serialize.hpp"

#include <string>

struct Person {
    // std::string name;
    int age;
};

auto main() -> int {
    // auto p = Person{"zhangsan", 18};
    auto        p = Person{0x7654321};
    std::string buf;
    fzto::serialize<Person, std::string>(p, buf);
    PRINT_FMT("serialize result : `{}` length: {}\n", buf.data(), buf.size());
    // auto another = deserialize<Person>(buf);
    // ASSERT(another.name == "zhangsan");
    // ASSERT(another.age == 18);
}
