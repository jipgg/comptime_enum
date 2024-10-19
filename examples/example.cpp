#include <comptime.hpp>
#include <iostream>
comptime_SENTINEL_ENUM(Sentinel_terminated_enum, Zero, One, Two, Three, Four, Five, Six);
enum class My_enum_class {Zero, One, Two, Three, Four, Five, Six, comptime_SENTINEL};
enum My_enum {Zero, One, Two, Three, Four, Five, Six, comptime_SENTINEL};
enum class Normal_enum_class {A, B, C, D, E, F, G, X, Y, Z};
comptime_ENUM(My_comptime_enum, Hello, World, One, Two, Three);
constexpr std::size_t a = comptime::count<My_comptime_enum>();
struct My_struct {
    int a;
    int b;
    float c;
    double d;
};

int main() {
    constexpr My_struct e{0, 0, 0, 0};
    std::cout << comptime::reflect<My_struct, e>();
    /*
    comptime::inline_for<comptime::count<Sentinel_terminated_enum>()>([](auto i) {
        const auto info = comptime::enum_info<My_enum_class, i>();
        std::cout << info.type << ": " << info.name << " (" << info.index <<")\n";
    });
    std::cout << "Normal_enum_class: ";
    constexpr auto count = comptime::count<Normal_enum_class, Normal_enum_class::Z>();
    constexpr auto normal_arr = comptime::to_array<Normal_enum_class, count>();
    for (auto& info : normal_arr) {
        std::cout << info.name << ", ";
    }
    constexpr auto array = comptime::to_array<My_enum_class>();
    std::cout << "\nSentinel_terminated_enum: ";
    for (auto& info : array) {
        std::cout << info.name << ", ";
    }
    auto item = comptime::enum_item<My_enum>("Six");
    std::cout << "\n" << int(item.to_enum()) << "\n"; 
    std::cout << comptime::enum_item<My_comptime_enum, 0>().name << " ";
    std::cout << comptime::enum_item<My_comptime_enum, 1>().name << "!\n";
    */
}
