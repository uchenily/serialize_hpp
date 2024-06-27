#include "serialize_hpp/aggregate_arity.hpp"
#include "serialize_hpp/debug.hpp"
#include <cstring>
#include <type_traits>
#include <utility>
namespace fzto {

namespace detail {
    template <typename Value, typename Container>
    auto serialize_to(const Value &val, Container &container) {
        // auto start = reinterpret_cast<const char *>(&val);
        // for (auto i = 0u; i < sizeof(val); i++) {
        //     // PRINT("start[{}]=0x{:02x}", i, (unsigned int) start[i]);
        //     container.push_back(start[i]);
        // }

        if constexpr (std::is_same_v<Value, std::string>) {
            // length + payload
            const auto origin_pos = container.size();
            uint32_t   length = val.size();
            container.resize(origin_pos + 4 + length);
            std::memcpy(container.data() + origin_pos, &length, 4);
            std::memcpy(container.data() + origin_pos + 4, val.data(), length);
        } else if constexpr (std::is_arithmetic_v<Value>) {
            const auto origin_pos = container.size();
            container.resize(origin_pos + sizeof(val));
            std::memcpy(container.data() + origin_pos, &val, sizeof(val));
        } else {
            static_assert(false, "Unsupported type");
        }
    }
    template <typename Value, typename Container, std::size_t... Indexes>
    auto serialize_helper(const Value &val,
                          Container   &container,
                          std::index_sequence<Indexes...> /*unused*/) {
        (serialize_to(fzto::get<Indexes>(val), container), ...);
    }
} // namespace detail

template <typename Container, typename Value>
    requires requires(Container c, typename Container::value_type v) {
        { c.push_back(v) } -> std::same_as<void>;
    } && std::is_class_v<Value>
auto serialize(const Value &val) {
    auto container = Container{};

    constexpr auto N = num_fields<Value>();
    detail::serialize_helper(val, container, std::make_index_sequence<N>{});
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

        if constexpr (std::is_same_v<Value, std::string>) {
            // length + payload
            auto     data = container.data();
            uint32_t length
                = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
            PRINT("string length: {}", length);
            val = std::string{container.data() + 4,
                              container.data() + 4 + length};
            pos += 4 + length;
        } else if constexpr (std::is_arithmetic_v<Value>) {
            std::memcpy(start, container.data() + pos, sizeof(val));
            pos += sizeof(val);
        } else {
            static_assert(false, "Unsupported type");
        }
    }
    template <typename Value, typename Container, std::size_t... Indexes>
    auto deserialize_helper(Value           &val,
                            const Container &container,
                            std::size_t      pos,
                            std::index_sequence<Indexes...> /*unused*/) {
        (deserialize_from(fzto::get<Indexes>(val), container, pos), ...);
    }
} // namespace detail

template <typename Value, typename Container>
    requires requires(Container c, std::size_t i) {
        { c[i] } -> std::convertible_to<typename Container::value_type>;
    } && std::is_class_v<Value>
auto deserialize(Container &container) {
    auto           val = Value{};
    constexpr auto N = num_fields<Value>();
    detail::deserialize_helper(val,
                               container,
                               0,
                               std::make_index_sequence<N>{});
    return val;
}
} // namespace fzto
