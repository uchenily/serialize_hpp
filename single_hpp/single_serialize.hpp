// DO NOT EDIT !!! This file was generated with a script.
//
// Generator Version: 0.0.1
// Generated Time: 2024-07-03 10:31:42.089234

#pragma once

// C++
#include <array>
#include <chrono>
#include <format>
#include <iostream>
#include <optional>
#include <source_location>
#include <span>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>
#include <variant>
#include <vector>

namespace print_hpp::log {
enum class LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
};
enum class LogStyle {
    FG,
    BG,
};
namespace detail {
    constexpr auto to_string(LogLevel level) noexcept -> std::string_view {
        using enum LogLevel;
        switch (level) {
        case TRACE:
            return "TRACE";
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO ";
        case WARN:
            return "WARN ";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
        }
    }
    constexpr auto to_color_fg(LogLevel level) noexcept -> std::string_view {
        using enum LogLevel;
        switch (level) {
        case TRACE:
            return "\033[36m";
        case DEBUG:
            return "\033[94m";
        case INFO:
            return "\033[32m";
        case WARN:
            return "\033[33m";
        case ERROR:
            return "\033[31m";
        case FATAL:
            return "\033[35m";
        default:
            return "";
        }
    }
    constexpr auto to_color_bg(LogLevel level) noexcept -> std::string_view {
        switch (level) {
            using enum LogLevel;
        case TRACE:
            return "\033[97;46m"; // cyan
        case DEBUG:
            return "\033[97;44m"; // blue
        case INFO:
            return "\033[97;42m"; // green
        case WARN:
            return "\033[90;43m"; // yellow
        case ERROR:
            return "\033[97;41m"; // red
        case FATAL:
            return "\033[97;45m"; // purple
        default:
            return "";
        }
    }
    constexpr auto to_color(LogLevel level, LogStyle style) noexcept
        -> std::string_view {
        using enum LogStyle;
        switch (style) {
        case FG:
            return to_color_fg(level);
        case BG:
            return to_color_bg(level);
        default:
            return "";
        }
    }
    template <LogLevel Level>
    auto function_name(std::source_location loc) noexcept -> std::string {
        if constexpr (Level <= LogLevel::DEBUG) {
            return std::format("|{}|", loc.function_name());
        } else {
            return {};
        }
    }
    constexpr auto source_color() noexcept -> std::string_view {
        return "\033[90m";
    }
    constexpr auto reset_color() noexcept -> std::string_view {
        return "\033[0m";
    }
    template <int N, char c>
    inline auto to_int(uint64_t num, std::span<char> p, int &size) {
        constexpr static std::array<char, 10> digits
            = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        for (int i = 0; i < N; i++) {
            p[--size] = digits.at(num % 10);
            num = num / 10;
        }
        if constexpr (N != 4) {
            p[--size] = c;
        }
    }
    inline auto get_timestamp(const std::chrono::system_clock::time_point &now)
        -> char * {
        static thread_local std::array<char, 33> buf{};
        static thread_local std::chrono::seconds last_second{};
        std::chrono::system_clock::duration duration = now.time_since_epoch();
        std::chrono::seconds                seconds
            = std::chrono::duration_cast<std::chrono::seconds>(duration);
        auto milliseconds
            = std::chrono::duration_cast<std::chrono::milliseconds>(duration
                                                                    - seconds)
                  .count();
        int size = 23;
        if (last_second == seconds) {
            to_int<3, '.'>(milliseconds, buf, size);
            return buf.data();
        }
        last_second = seconds;
        auto tt = std::chrono::system_clock::to_time_t(now);
        auto tm = localtime(&tt);
        to_int<3, '.'>(milliseconds, buf, size);
        to_int<2, ':'>(tm->tm_sec, buf, size);
        to_int<2, ':'>(tm->tm_min, buf, size);
        to_int<2, ' '>(tm->tm_hour, buf, size);
        to_int<2, '-'>(tm->tm_mday, buf, size);
        to_int<2, '-'>(tm->tm_mon + 1, buf, size);
        to_int<4, ' '>(tm->tm_year + 1900, buf, size);
        return buf.data();
    }
    class FmtWithSourceLocation {
    public:
        template <typename T>
            requires std::constructible_from<std::string_view, T>
        FmtWithSourceLocation(T                  &&fmt,
                              std::source_location location
                              = std::source_location::current())
            : fmt_{std::forward<T>(fmt)}
            , source_location_{location} {}

    public:
        auto fmt() {
            return fmt_;
        }
        auto source_location() {
            return source_location_;
        }

    private:
        std::string_view     fmt_;
        std::source_location source_location_;
    };
    class ConsoleLogger {
    public:
        template <typename... Args>
        auto trace(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::TRACE>(fwsl, std::forward<Args>(args)...);
        }
        template <typename... Args>
        auto debug(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::DEBUG>(fwsl, std::forward<Args>(args)...);
        }
        template <typename... Args>
        auto info(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::INFO>(fwsl, std::forward<Args>(args)...);
        }
        template <typename... Args>
        auto warn(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::WARN>(fwsl, std::forward<Args>(args)...);
        }
        template <typename... Args>
        auto error(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::ERROR>(fwsl, std::forward<Args>(args)...);
        }
        template <typename... Args>
        auto fatal(FmtWithSourceLocation fwsl, Args &&...args) {
            log<LogLevel::FATAL>(fwsl, std::forward<Args>(args)...);
        }
        template <typename Fmt, typename... Args>
            requires std::constructible_from<std::string_view, Fmt>
        auto printf(Fmt fmt, Args... args) {
            std::clog << std::vformat(fmt, std::make_format_args(args...));
        }
        template <typename Fmt, typename... Args>
            requires std::constructible_from<std::string_view, Fmt>
        auto printfln(Fmt fmt, Args... args) {
            std::clog << std::vformat(fmt, std::make_format_args(args...))
                      << '\n';
        }
        auto set_level(LogLevel level) {
            level_ = level;
        }
        auto set_style(LogStyle style) {
            style_ = style;
        }

    private:
        template <LogLevel Level, typename... Args>
        auto log(FmtWithSourceLocation fwsl, const Args &...args) {
            if (Level < level_) {
                return;
            }
            auto fmt = fwsl.fmt();
            auto source_location = fwsl.source_location();
            auto message = std::vformat(fmt, std::make_format_args(args...));
            auto now = std::chrono::system_clock::now();
            std::clog << std::format("{} |{}{:<5}{}| {}{}:{}{}{} {}\n",
                                     get_timestamp(now),
                                     to_color(Level, style_),
                                     to_string(Level),
                                     reset_color(),
                                     source_color(),
                                     source_location.file_name(),
                                     source_location.line(),
                                     function_name<Level>(source_location),
                                     reset_color(),
                                     message);
        }

    private:
        LogLevel level_{LogLevel::DEBUG};
        LogStyle style_{LogStyle::FG};
    };
} // namespace detail
inline auto console = detail::ConsoleLogger();
} // namespace print_hpp::log

inline auto &debug_logger = print_hpp::log::console;
#define PRINT_NL(...) debug_logger.printf(__VA_ARGS__)
#define PRINT(...) debug_logger.printfln(__VA_ARGS__)
#if !defined(NDEBUG)
#define SET_LOG_LEVEL(level) debug_logger.set_level(level)
#define SET_LOG_STYLE(style) debug_logger.set_style(style)
#define LOG_TRACE(...) debug_logger.trace(__VA_ARGS__)
#define LOG_INFO(...) debug_logger.info(__VA_ARGS__)
#define LOG_DEBUG(...) debug_logger.debug(__VA_ARGS__)
#define LOG_WARN(...) debug_logger.warn(__VA_ARGS__)
#define LOG_ERROR(...) debug_logger.error(__VA_ARGS__)
#define LOG_FATAL(...) debug_logger.fatal(__VA_ARGS__)
#else
#define SET_LOG_LEVEL(level)
#define SET_LOG_STYLE(style)
#define LOG_TRACE(...)
#define LOG_INFO(...)
#define LOG_DEBUG(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_FATAL(...)
#endif
#if !defined(NDEBUG)
#include <cassert>
#define ASSERT(x) assert(x)
#define ASSERT_MSG(x, msg) ASSERT((x) && (msg))
#else
#define ASSERT(x)
#define ASSERT_MSG(x, msg)
#endif

#include <ostream>
#include <ranges>
#include <sstream>

namespace print_custom {
template <typename T>
struct printer {
    printer() = delete;
    printer(const printer &) = delete;
    auto operator=(const printer &) = delete;
};
template <typename T>
inline auto print_to(std::ostream &out, T &&t);
} // namespace print_custom
namespace print_detail {
template <typename T>
constexpr auto name_of() -> std::string_view {
    std::string_view sv;
#if __clang__
    sv = __PRETTY_FUNCTION__;
    sv = sv.substr(sv.find_last_of('=') + 2);
    sv = sv.substr(0, sv.size() - 1);
#elif __GNUC__
    sv = __PRETTY_FUNCTION__;
    sv = sv.substr(sv.find_first_of('=') + 2);
    sv = sv.substr(0, sv.find_first_of(';'));
#else
#error "name_of not implemented on this platform"
#endif
    return sv;
}
template <typename Container>
concept is_iterable_v = requires(Container &c) {
    std::ranges::begin(c);
    std::ranges::end(c);
};
template <typename Container>
concept is_mappable_v = requires(Container &c) {
    typename Container::key_type;
    typename Container::mapped_type;
    {
        c[std::declval<const typename Container::key_type &>()]
    } -> std::same_as<typename Container::mapped_type &>;
};
template <typename T>
concept is_variant_v = requires { std::variant_size<T>::value; };
template <typename T>
concept is_optional_v = requires(T &t) {
    { t.has_value() } -> std::same_as<bool>;
    { t.value() } -> std::convertible_to<T>;
};
template <typename Container>
concept is_tuple_v = requires { std::tuple_size<Container>::value; };
template <typename T>
concept has_printer_print_v = requires(print_custom::printer<T> &printer,
                                       const T                  &t,
                                       std::ostream             &out) {
    { printer.print(t, out) };
};
inline auto to_visible(std::string_view s) {
    std::ostringstream oss;
    for (char c : s) {
        if (std::isprint(static_cast<unsigned char>(c)) != 0) {
            oss << c;
        } else {
            oss << '.';
        }
    }
    return oss.str();
}
template <typename T>
auto print_to(std::ostream &out, const T &t) {
    if constexpr (std::is_convertible_v<T, std::string_view>) {
        // out << '"' << t << '"';
        out << '"' << to_visible(t) << '"';
    } else if constexpr ((std::is_integral_v<T> && !std::is_same_v<T, bool>)
                         || std::is_floating_point_v<T>) {
        out << t;
    } else if constexpr (std::is_same_v<T, bool>) {
        out << (t ? "true" : "false");
    } else if constexpr (is_mappable_v<T>) {
        out << '{';
        auto first = true;
        for (const auto &elem : t) {
            if (!first) {
                out << ", ";
            }
            first = false;
            print_to(out, elem.first);
            out << ": ";
            print_to(out, elem.second);
        }
        out << "}";
    } else if constexpr (is_iterable_v<T>) {
        out << '[';
        auto first = true;
        for (const auto &elem : t) {
            if (!first) {
                out << ", ";
            }
            first = false;
            print_to(out, elem);
        }
        out << "]";
    } else if constexpr (is_variant_v<T>) {
        std::visit(
            [&out](const auto &value) {
                print_to(out, value);
            },
            t);
    } else if constexpr (is_optional_v<T>) {
        if (t) {
            print_to(out, t.value());
        } else {
            out << "nullopt";
        }
    } else if constexpr (std::is_same_v<T, std::nullopt_t>) {
        out << "nullopt";
    } else if constexpr (is_tuple_v<T>) {
        out << '(';
        auto first = true;
        std::apply(
            [&out, &first](auto &&...args) {
                (([&out, &first, &args] {
                     if (!first) {
                         out << ", ";
                     }
                     print_to(out, std::forward<decltype(args)>(args));
                     first = false;
                 }()),
                 ...);
            },
            t);
        out << ')';
    } else if constexpr (has_printer_print_v<T>) {
        print_custom::printer<T>().print(t, out);
    } else {
        out << "unprintable type " << name_of<T>();
    }
}
} // namespace print_detail
template <typename T>
auto print_custom::print_to(std::ostream &out, T &&t) {
    print_detail::print_to(out, std::forward<T>(t));
}
namespace print_hpp {
template <typename T>
auto print(T &&t) {
    print_detail::print_to(std::cout, std::forward<T>(t));
    std::cout << '\n';
}
template <typename T, typename... Ts>
void print(T &&t, Ts &&...ts) {
    print_detail::print_to(std::cout, std::forward<T>(t));
    ((std::cout << ' ',
      print_detail::print_to(std::cout, std::forward<Ts>(ts))),
     ...);
    std::cout << '\n';
}
// pretty print
template <typename T>
auto P(T &&t) {
    std::ostringstream oss; // 千万别用 `std::ostrstream` !
    print_detail::print_to(oss, std::forward<T>(t));
    return oss.str();
}
} // namespace print_hpp

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
// clang-format off
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
            return std::get<index>(std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8));
        } else if constexpr (arity == 10) {
            auto &[p0, p1, p2, p3, p4, p5, p6, p7, p8, p9] = value;
            return std::get<index>(std::tie(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9));
        } else {
            /* extend it by yourself for higher arities */
            static_assert(false, "Too many arities!");
        }
    } else {
        static_assert(index < arity, "Index out of bounds");
    }
}
// clang-format on
template <typename T>
constexpr auto num_fields() noexcept -> std::size_t {
    // FIXME: 为什么换成 std::decay 就出现问题?
    return aggregate_arity<
        std::remove_reference_t<std::remove_cv_t<T>>>::size();
}
} // namespace fzto

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

#include <cstring>
namespace fzto {
template <typename Value>
concept is_iterable_v = requires(Value &v) {
    std::ranges::begin(v);
    std::ranges::end(v);
};
namespace detail {
    template <Arithmetic Value, typename Container>
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
    template <String Value, typename Container>
    auto serialize_to(const Value &val, Container &container) {
        // length + payload
        const auto origin_pos = container.size();
        uint32_t   length = val.size();
        container.resize(origin_pos + 4 + length);
        std::memcpy(container.data() + origin_pos, &length, 4);
        std::memcpy(container.data() + origin_pos + 4, val.data(), length);
    }
    template <Vector Value, typename Container>
    auto serialize_to(const Value &val, Container &container) {
        // size + size * item
        const auto origin_pos = container.size();
        container.resize(origin_pos + 4);
        uint32_t size = val.size();
        std::memcpy(container.data() + origin_pos, &size, 4);
        for (auto i = 0u; i < val.size(); i++) {
            serialize_to(val[i], container);
        }
    }
    template <Array Value, typename Container>
    auto serialize_to(const Value &val, Container &container) {
        // size * item
        for (auto i = 0u; i < val.size(); i++) {
            serialize_to(val[i], container);
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
    auto           container = Container{};
    constexpr auto N = num_fields<Value>();
    detail::serialize_helper(val, container, std::make_index_sequence<N>{});
    return container;
}
namespace detail {
    template <Arithmetic Value, typename Container>
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
    template <String Value, typename Container>
    auto
    deserialize_from(Value &val, const Container &container, std::size_t &pos) {
        // length + payload
        auto     data = container.data();
        uint32_t length
            = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
        PRINT("string length: {}", length);
        val = std::string{container.data() + 4, container.data() + 4 + length};
        pos += 4 + length;
    }
    template <Vector Value, typename Container>
    auto
    deserialize_from(Value &val, const Container &container, std::size_t &pos) {
        // size + size * item
        auto     data = container.data();
        uint32_t size = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
        pos += 4;
        PRINT("std::vector length: {}", size);
        if (val.size() < size) {
            val.resize(size);
        }
        for (auto i = 0u; i < size; i++) {
            deserialize_from(val[i], container, pos);
        }
    }
    template <Array Value, typename Container>
    auto
    deserialize_from(Value &val, const Container &container, std::size_t &pos) {
        // size * item
        PRINT("std::array length: {}", val.size());
        for (auto i = 0u; i < val.size(); i++) {
            deserialize_from(val[i], container, pos);
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
