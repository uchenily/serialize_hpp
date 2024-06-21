#include <tuple>
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
    if constexpr (index < arity) {
        if constexpr (arity == 1) {
            auto &[p0] = value;
            return std::get<index>(std::tie(p0));
        } else if constexpr (arity == 2) {
            auto &[p0, p1] = value;
            return std::get<index>(std::tie(p0, p1));
        } else if constexpr (arity == 3) {
            auto &[p0, p1, p2] = value;
            return std::get<index>(std::tie(p0, p1, p2));
        } else if constexpr (arity == 4) {
            auto &[p0, p1, p2, p3] = value;
            return std::get<index>(std::tie(p0, p1, p2, p3));
        } else if constexpr (arity == 5) {
            auto &[p0, p1, p2, p3, p4] = value;
            return std::get<index>(std::tie(p0, p1, p2, p3, p4));
        } else if constexpr (arity == 6) {
            auto &[p0, p1, p2, p3, p4, p5] = value;
            return std::get<index>(std::tie(p0, p1, p2, p3, p4, p5));
        } else if constexpr (arity == 7) {
            auto &[p0, p1, p2, p3, p4, p5, p6] = value;
            return std::get<index>(std::tie(p0, p1, p2, p3, p4, p5, p6));
        } else if constexpr (arity == 8) {
            auto &[p0, p1, p2, p3, p4, p5, p6, p7] = value;
            return std::get<index>(std::tie(p0, p1, p2, p3, p4, p5, p6, p7));
        } else if constexpr (arity == 9) {
            auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8] = value;
            return std::get<index>(
                std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8));
        } else if constexpr (arity == 10) {
            auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9] = value;
            return std::get<index>(
                std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9));
        } else {
            /* extend it by yourself for higher arities */
            static_assert(false, "Too many arities!");
        }
    } else {
        static_assert(index < arity, "Index out of bounds");
    }
}

template <typename T>
constexpr auto num_fields() noexcept -> std::size_t {
    // FIXME: 为什么换成 std::decay 就出现问题?
    return aggregate_arity<
        std::remove_reference_t<std::remove_cv_t<T>>>::size();
}

} // namespace fzto
