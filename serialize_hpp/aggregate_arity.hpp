#include <type_traits>
#include <utility>

namespace fzto {

// https://codereview.stackexchange.com/questions/142804/get-n-th-data-member-of-a-struct

struct filler {
    template <typename type>
    operator type();
};

template <typename aggregate,
          typename index_sequence = std::index_sequence<>,
          typename = void>
struct aggregate_arity : index_sequence {};

template <typename aggregate, std::size_t... indices>
struct aggregate_arity<
    aggregate,
    std::index_sequence<indices...>,
    std::void_t<decltype(aggregate{(indices, std::declval<filler>())...,
                                   std::declval<filler>()})>>
    : aggregate_arity<aggregate,
                      std::index_sequence<indices..., sizeof...(indices)>> {};

template <std::size_t index, typename type>
constexpr auto get(type &value) noexcept -> decltype(auto) {
    constexpr std::size_t arity = aggregate_arity<
        std::remove_reference_t<std::remove_cv_t<type>>>::size();
    if constexpr (arity == 1) {
        auto &[p1] = value;
        if constexpr (index == 0) {
            return (p1);
        } else {
            return;
        }
    } else if constexpr (arity == 2) {
        auto &[p1, p2] = value;
        if constexpr (index == 0) {
            return (p1);
        } else if constexpr (index == 1) {
            return (p2);
        } else {
            return;
        }
    } else if constexpr (arity == 3) {
        auto &[p1, p2, p3] = value;
        if constexpr (index == 0) {
            return (p1);
        } else if constexpr (index == 1) {
            return (p2);
        } else if constexpr (index == 2) {
            return (p3);
        } else {
            return;
        }
    } else /* extend it by yourself for higher arities */ {
        return;
    }
}

template <typename T>
constexpr auto num_fields() noexcept -> std::size_t {
    return aggregate_arity<
        std::remove_reference_t<std::remove_cv_t<T>>>::size();
}

} // namespace fzto
