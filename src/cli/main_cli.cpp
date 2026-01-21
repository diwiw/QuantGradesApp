#include <iostream>

#include "Version.hpp"
#include "cli/AppCLI.hpp"

int main(int argc, char** argv)
{
    // === Header ===
    std::cout << "===================================\n";
    std::cout << " QuantGradesApp Logger Demo\n";
    std::cout << " Version: " << APP_VERSION << "\n";
    std::cout << " Build date: " << APP_BUILD_DATE << "\n";
    std::cout << "===================================\n\n";

    qga::cli::AppCLI app;
    return app.run(argc, argv);
}
