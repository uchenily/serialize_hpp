#include "serialize_hpp/aggregate_arity.hpp"
#include "serialize_hpp/concept.hpp"
#include "serialize_hpp/debug.hpp"

#include <cstring>

namespace fzto {

template <typename Value>
concept is_iterable_v = requires(Value &v) {
    std::ranges::begin(v);
    std::ranges::end(v);
};

namespace detail {
    template <Arithmetic Value, typename WriteBuffer>
    auto serialize_to(const Value &val, WriteBuffer &buf) {
        // auto start = reinterpret_cast<const char *>(&val);
        // for (auto i = 0u; i < sizeof(val); i++) {
        //     // PRINT("start[{}]=0x{:02x}", i, (unsigned int) start[i]);
        //     buf.push_back(start[i]);
        // }
        const auto origin_pos = buf.size();
        buf.resize(origin_pos + sizeof(val));
        std::memcpy(buf.data() + origin_pos, &val, sizeof(val));
    }

    template <String Value, typename WriteBuffer>
    auto serialize_to(const Value &val, WriteBuffer &buf) {
        // length + payload
        const auto origin_pos = buf.size();
        uint32_t   length = val.size();
        buf.resize(origin_pos + 4 + length);
        std::memcpy(buf.data() + origin_pos, &length, 4);
        std::memcpy(buf.data() + origin_pos + 4, val.data(), length);
    }

    template <Vector Value, typename WriteBuffer>
    auto serialize_to(const Value &val, WriteBuffer &buf) {
        // size + size * item
        const auto origin_pos = buf.size();
        buf.resize(origin_pos + 4);

        uint32_t size = val.size();
        std::memcpy(buf.data() + origin_pos, &size, 4);

        for (auto i = 0u; i < val.size(); i++) {
            serialize_to(val[i], buf);
        }
    }

    template <Array Value, typename WriteBuffer>
    auto serialize_to(const Value &val, WriteBuffer &buf) {
        // size * item
        for (auto i = 0u; i < val.size(); i++) {
            serialize_to(val[i], buf);
        }
    }

    template <Enum Value, typename WriteBuffer>
    auto serialize_to(const Value &val, WriteBuffer &buf) {
        const auto origin_pos = buf.size();
        buf.resize(origin_pos + sizeof(val));
        std::memcpy(buf.data() + origin_pos, &val, sizeof(val));
    }

    template <typename Value, typename WriteBuffer, std::size_t... Indexes>
    auto serialize_helper(const Value &val,
                          WriteBuffer &buf,
                          std::index_sequence<Indexes...> /*unused*/) {
        (serialize_to(fzto::get<Indexes>(val), buf), ...);
    }
} // namespace detail

template <typename WriteBuffer, typename Value>
    requires requires(WriteBuffer                      c,
                      typename WriteBuffer::value_type v,
                      std::size_t                      s) {
        { c.push_back(v) } -> std::same_as<void>;
        { c.resize(s) } -> std::same_as<void>;
    } && std::is_class_v<Value>
auto serialize(const Value &val) {
    auto buf = WriteBuffer{};

    constexpr auto N = num_fields<Value>();
    detail::serialize_helper(val, buf, std::make_index_sequence<N>{});
    return buf;
}

namespace detail {
    template <Arithmetic Value, typename ReadBuffer>
    auto deserialize_from(Value &val, const ReadBuffer &buf, std::size_t &pos) {
        auto start = reinterpret_cast<char *>(&val);
        // for (auto i = 0u; i < sizeof(val); i++) {
        //     start[i] = buf[pos++];
        //     PRINT("start[{}]=0x{:02x}", i, (unsigned int) start[i]);
        // }

        std::memcpy(start, buf.data() + pos, sizeof(val));
        pos += sizeof(val);
    }

    template <String Value, typename ReadBuffer>
    auto deserialize_from(Value &val, const ReadBuffer &buf, std::size_t &pos) {
        // length + payload
        auto     data = buf.data();
        uint32_t length
            = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
        PRINT("string length: {}", length);
        val = std::string{buf.data() + 4, buf.data() + 4 + length};
        pos += 4 + length;
    }

    template <Vector Value, typename ReadBuffer>
    auto deserialize_from(Value &val, const ReadBuffer &buf, std::size_t &pos) {
        // size + size * item
        auto     data = buf.data();
        uint32_t size = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
        pos += 4;
        PRINT("std::vector length: {}", size);
        if (val.size() < size) {
            val.resize(size);
        }
        for (auto i = 0u; i < size; i++) {
            deserialize_from(val[i], buf, pos);
        }
    }

    template <Array Value, typename ReadBuffer>
    auto deserialize_from(Value &val, const ReadBuffer &buf, std::size_t &pos) {
        // size * item
        PRINT("std::array length: {}", val.size());
        for (auto i = 0u; i < val.size(); i++) {
            deserialize_from(val[i], buf, pos);
        }
    }

    template <Enum Value, typename ReadBuffer>
    auto deserialize_from(Value &val, const ReadBuffer &buf, std::size_t &pos) {
        auto start = reinterpret_cast<char *>(&val);
        std::memcpy(start, buf.data() + pos, sizeof(val));
        pos += sizeof(val);
    }

    template <typename Value, typename ReadBuffer, std::size_t... Indexes>
    auto deserialize_helper(Value            &val,
                            const ReadBuffer &buf,
                            std::size_t       pos,
                            std::index_sequence<Indexes...> /*unused*/) {
        (deserialize_from(fzto::get<Indexes>(val), buf, pos), ...);
    }
} // namespace detail

template <typename Value, typename ReadBuffer>
    requires requires(ReadBuffer c, std::size_t i) {
        { c[i] } -> std::convertible_to<typename ReadBuffer::value_type>;
    } && std::is_class_v<Value>
auto deserialize(ReadBuffer &buf) {
    auto           val = Value{};
    constexpr auto N = num_fields<Value>();
    detail::deserialize_helper(val, buf, 0, std::make_index_sequence<N>{});
    return val;
}
} // namespace fzto
