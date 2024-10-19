#pragma once
#include <source_location>
#include <string_view>
namespace comptime {
template <class Struct, Struct value>
std::string_view reflect() {
    return std::source_location::current().function_name();
}
}
