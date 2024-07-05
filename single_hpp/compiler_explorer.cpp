#include "https://raw.githubusercontent.com/uchenily/serialize_hpp/main/single_hpp/single_serialize.hpp"

using namespace print_hpp;
using namespace fzto;

struct Person {
    std::string name;
    int         age;
};

auto main() -> int {
    auto p = Person{"zhangsan", 18};
    auto buf = serialize<std::string>(p);
    PRINT("serialize result: {} length: {}", P(buf), buf.size());

    auto another = deserialize<Person>(buf);
    ASSERT(another.name == "zhangsan");
    ASSERT(another.age == 18);
}
