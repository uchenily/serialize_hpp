#include "print.hpp"

#include <list>
#include <map>
#include <optional>
#include <string_view>
#include <vector>

auto main() -> int {
    print("hello");
    print(std::string{"hello"});
    print(std::string_view{"hello"});
    print(3);
    print(3.14);
    // print(std::vector{1, 2, 3});
    // print(std::list{1.0, 2.0, 3.0});
    // print(std::map<std::string, std::optional<std::string>>{
    //     {"hello", "world"},
    //     { "work",     996},
    // });
}
