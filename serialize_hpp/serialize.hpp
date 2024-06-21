#include "serialize_hpp/aggregate_arity.hpp"
#include "serialize_hpp/debug.hpp"
#include <cstring>
namespace fzto {

namespace detail {
    template <typename Value, typename Container>
    auto serialize_to(const Value &val, Container &container) {
        // auto start = reinterpret_cast<const char *>(&val);
        // for (auto i = 0u; i < sizeof(val); i++) {
        //     // PRINT("start[{}]=0x{:02x}", i, (unsigned int) start[i]);
        //     container.push_back(start[i]);
        // }

        const auto origin_pos = container.size();
        container.resize(origin_pos + sizeof(val));
        std::memcpy(container.data() + origin_pos, &val, sizeof(val));
    }
    template <typename Value, typename Container, std::size_t I, std::size_t N>
    auto serialize_helper(const Value &val, Container &container) {
        if constexpr (I < N) {
            auto &field = fzto::get<I>(val);
            serialize_to(field, container);
            serialize_helper<Value, Container, I + 1, N>(val, container);
        }
    }
} // namespace detail

template <typename Container, typename Value>
    requires requires(Container c, typename Container::value_type v) {
        { c.push_back(v) } -> std::same_as<void>;
    } && std::is_class_v<Value>
auto serialize(const Value &val) {
    auto container = Container{};

    constexpr auto N = num_fields<Value>();
    detail::serialize_helper<Value, Container, 0, N>(val, container);
    return container;
}

namespace detail {
    template <typename Value, typename Container>
    auto
    deserialize_from(Value &val, const Container &container, std::size_t &pos) {
        auto start = reinterpret_cast<char *>(&val);
        // for (auto i = 0u; i < sizeof(val); i++) {
        //     start[i] = container[pos++];
        //     PRINT("start[{}]=0x{:02x}", i, (unsigned int) start[i]);
        // }

        std::memcpy(start, container.data() + pos, sizeof(val));
        pos += sizeof(val);
    }
    template <typename Value, typename Container, std::size_t I, std::size_t N>
    auto deserialize_helper(Value           &val,
                            const Container &container,
                            std::size_t      pos) {
        if constexpr (I < N) {
            auto &field = fzto::get<I>(val);
            deserialize_from(field, container, pos);
            deserialize_helper<Value, Container, I + 1, N>(val, container, pos);
        }
    }
} // namespace detail

template <typename Value, typename Container>
    requires requires(Container c, std::size_t i) {
        { c[i] } -> std::convertible_to<typename Container::value_type>;
    } && std::is_class_v<Value>
auto deserialize(Container &container) {
    auto           val = Value{};
    constexpr auto N = num_fields<Value>();
    detail::deserialize_helper<Value, Container, 0, N>(val, container, 0);
    return val;
}
} // namespace fzto
