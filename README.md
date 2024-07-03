# serialize_hpp

A header-only C++20 serialization library


## Example

```cpp
#include "serialize_hpp/debug.hpp"
#include "serialize_hpp/serialize.hpp"

#include <string>

struct Person {
    std::string name;
    int         age;
};

auto main() -> int {
    auto p = Person{"zhangsan", 18};
    auto buf = fzto::serialize<std::string>(p);
    PRINT("serialize result : `{}` length: {}", buf.data(), buf.size());
    auto another = fzto::deserialize<Person>(buf);
    ASSERT(another.name == "zhangsan");
    ASSERT(another.age == 18);
}
```

## Try on godbolt

[compiler-explorer](https://godbolt.org/z/qs38c1erx)
