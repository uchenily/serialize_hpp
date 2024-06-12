#include "serialize_hpp/debug.hpp"

#include <string>

struct Person {
    std::string name;
    int         age;
};

auto main() -> int {
    auto p = Person{"zhangsan", 18};
    auto buf = serialize<std::string>(p);
    auto another = deserialize<Person>(buf);
    ASSERT(another.name == "zhangsan");
    ASSERT(another.age == 18);
}
