#pragma once
#include <source_location>
#include <string>
#include <type_traits>
#include <utility>
#include <string_view>
#include <array>
#define comptime_enum(Enum_type, ...) enum class Enum_type { __VA_ARGS__, _end }
template<class Val, Val v>
struct comptime_val {
    static const constexpr auto value = v;
    constexpr operator Val() const {return v;}
};
template <int v>
using comptime_int = comptime_val<int, v>;
template <class Func, std::size_t... indices>
constexpr void comptime_for(Func fn, std::index_sequence<indices...>) {
  (fn(comptime_val<std::size_t, indices>{}), ...);
}
template <std::size_t count, typename Func>
constexpr void comptime_for(Func fn) {
  comptime_for(fn, std::make_index_sequence<count>());
}
namespace comptime {
template <class T>
concept Enum = std::is_enum_v<T>;
template <Enum T>
consteval std::size_t count() {
    return static_cast<std::size_t>(T::_end);
}
struct Enum_info {
    std::string_view type;
    std::string_view value;
    std::string_view raw;
    int index;
};
namespace detail {
#if defined (_MSC_VER)//msvc compiler
template <Enum type, type value>
constexpr Enum_info enum_info() {
    const std::string_view raw{std::source_location::current().function_name()};
    const std::string enum_t_keyw{"<enum "};
    auto found = raw.find(enum_t_keyw);
    std::string_view enum_t = raw.substr(raw.find(enum_t_keyw) + enum_t_keyw.size());
    bool is_enum_class = enum_t.find("::") != std::string::npos;
    enum_t = enum_t.substr(0, enum_t.find_first_of(','));
    const std::string preceding = enum_t_keyw + std::string(enum_t) + ",";
    std::string_view enum_v = raw.substr(raw.find(preceding) + preceding.size());
    if (is_enum_class) {
        enum_v = enum_v.substr(enum_v.find("::") + 2);
    }
    enum_v = enum_v.substr(0, enum_v.find_first_of(">"));
    return {
        .type = enum_t,
        .value = enum_v,
        .raw = raw,
        .index = int(value),
    };
}
#elif defined(__clang__) || defined(__GNUC__)
template <Enum type, type value>
constexpr Enum_info enum_info() {
    using sv = std::string_view;
    const sv raw{std::source_location::current().function_name()};
    const sv enum_find{"type = "}; 
    const sv value_find{"value = "};
    sv enum_t = raw.substr(raw.find(enum_find) + enum_find.size());
    bool is_enum_class = enum_t.find("::") != std::string::npos;
    enum_t = enum_t.substr(0, enum_t.find(";"));
    sv enum_v = raw.substr(raw.find(value_find) + value_find.size());
    if (is_enum_class) {
        enum_v = enum_v.substr(enum_v.find("::") + 2);
    }
    enum_v = enum_v.substr(0, enum_v.find("]"));
    return {
        .type = enum_t,
        .value = enum_v,
        .raw = raw,
        .index = int(value),
    };
}
#else
static_assert(false, "platform not supported")
#endif
}
template <Enum type, int value>
constexpr Enum_info enum_info() {
    return detail::enum_info<type, static_cast<type>(value)>();
}
template <Enum T> 
constexpr std::array<Enum_info, count<T>()> to_array() {
    std::array<Enum_info, count<T>()> arr{};
    comptime_for<count<T>()>([&arr](auto i) {
        arr[i] = enum_info<T, i>();
    });
    return arr;
}
}
