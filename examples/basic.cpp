#include <klyro/klyro.hpp>
#include <iostream>

int main() {
    std::cout << "KlyroDB Version: " << klyro::version() << "\n";

    auto result = klyro::Database::open("example.klyro");

    if (!result) {
        std::cerr << "Expected failure (Module 1): " << klyro::to_string(result.error()) << "\n";
        return 0; // Returning 0 since this failure is expected for Module 1
    }

    // Unreachable in Module 1
    auto db = std::move(result.value());
    db.close();

    return 0;
}
