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

template <typename Value, typename Container>
auto serialize(const Value &val, Container &container) {
    constexpr std::size_t N
        = fzto::aggregate_arity<std::remove_cv_t<Value>>::size();
    detail::serialize_helper<Value, Container, 0, N>(val, container);
}
} // namespace fzto
