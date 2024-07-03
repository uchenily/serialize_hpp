#include <string>
#include <type_traits>
#include <vector>

template <typename T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <typename T>
concept String = std::is_same_v<T, std::string>;

template <typename T>
concept Vector = std::is_same_v<T, std::vector<typename T::value_type>>;

// template <typename T, std::size_t N>
// concept Array = std::is_same_v<T, std::array<typename T::value_type, N>>;
template <typename T>
concept Array = std::
    is_same_v<T, std::array<typename T::value_type, std::tuple_size<T>::value>>;
