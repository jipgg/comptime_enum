#pragma once
#include <source_location>
#include <string>
#include <type_traits>
#include <string_view>
#include <array>
#include <cassert>
#include <ranges>
#include "comptime.common.h"
namespace comptime {
#define comptime_SENTINEL _end
template <class T>
concept Enum = std::is_enum_v<T>;
template <Enum Type>
consteval std::size_t count() {
    return static_cast<std::size_t>(Type::comptime_SENTINEL);
}
#define comptime_SENTINEL_ENUM(Enum_type, ...) enum class Enum_type {__VA_ARGS__, comptime_SENTINEL}
#define comptime_ENUM(Enum_type, ...) enum class Enum_type {__VA_ARGS__};\
template <>\
consteval std::size_t comptime::count<Enum_type>() {\
    return comptime_COUNT_VA_ARGS(__VA_ARGS__);\
}
template <Enum Type, Type last_item>
consteval std::size_t count() {
    return static_cast<std::size_t>(last_item) + 1;
}
struct Enum_info {
    std::string_view type;
    std::string_view name;
    std::string_view raw;
    int index;
};
template <Enum Type>
struct Enum_item {
    std::string_view name;
    int index;
    constexpr Type to_enum() const {return Type(index);}
    constexpr operator Type() const {return Type(index);}
};
namespace detail {
#if defined (_MSC_VER)//msvc compiler
template <Enum Type, Type value>
consteval Enum_info enum_info() {
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
        .name = enum_v,
        .raw = raw,
        .index = int(value),
    };
}
#elif defined(__clang__) || defined(__GNUC__)
template <Enum Type, Type value>
consteval Enum_info enum_info() {
    using sv = std::string_view;
    const sv raw{std::source_location::current().function_name()};
    const sv enum_find{"Type = "}; 
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
        .name = enum_v,
        .raw = raw,
        .index = int(value),
    };
}
#else
static_assert(false, "platform not supported")
#endif
}
template <Enum Type, int value>
consteval Enum_info enum_info() {
    return detail::enum_info<Type, static_cast<Type>(value)>();
}
template <Enum Type, int value>
constexpr Enum_item<Type> enum_item() {
    constexpr auto info = enum_info<Type, value>();
    return Enum_item<Type>{
        .name = info.name,
        .index = value,
    };
}
template <Enum Type, int size = count<Type>()> 
consteval std::array<Enum_info, size> to_array() {
    std::array<Enum_info, size> arr{};
    inline_for<size>([&arr](auto i) {
        arr[i] = enum_info<Type, i>();
    });
    return arr;
}
template <Enum Type, int size = count<Type>()>
constexpr Enum_item<Type> enum_item(std::string_view name) {
    constexpr auto array = to_array<Type, size>();
    auto found_it = std::ranges::find_if(array, [&name](const Enum_info& e) {return e.name == name;});
    assert(found_it != std::ranges::end(array));
    return {.name = found_it->name, .index = found_it->index};
}
}
