#include <comptime_enum.h>
#include <iostream>
//fix for enum also
comptime_enum(Comptime_enum, Zero, One, Two, Three, Four, Five, Six);
enum class My_enum_class {Zero, One, Two, Three, Four, Five, Six, _end};
enum My_enum {Zero, One, Two, Three, Four, Five, Six, _end};

int main() {
    comptime_for<comptime::count<Comptime_enum>()>([](auto i) {
        const auto info = comptime::enum_info<My_enum_class, i>();
        std::cout << info.type << ": " << info.value << " (" << info.index <<")\n";
    });
    auto a = comptime::to_array<Comptime_enum>();
    std::cout << "Comptime_enum:\n";
    for (auto& b : a) {
        std::cout << "\t" << b.value << "\n";
    }
}
