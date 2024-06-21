#include "serialize_hpp/aggregate_arity.hpp"
#include "serialize_hpp/debug.hpp"
namespace fzto {

namespace detail {
    template <typename Value, typename Container>
    auto serialize_to(const Value &val, Container &container) {
        auto start = reinterpret_cast<const char *>(&val);
        for (auto i = 0u; i < sizeof(val); i++) {
            LOG_DEBUG("start[{}]=0x{:02x}", i, (int) start[i]);
            container.push_back(start[i]);
        }
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
        for (auto i = 0u; i < sizeof(val); i++) {
            start[i] = container[pos++];
            LOG_DEBUG("start[{}]=0x{:02x}", i, (int) start[i]);
        }
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
auto deserialize(Container &container) {
    auto           val = Value{};
    constexpr auto N = num_fields<Value>();
    detail::deserialize_helper<Value, Container, 0, N>(val, container, 0);
    return val;
}
} // namespace fzto
