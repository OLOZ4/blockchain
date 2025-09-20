#include <fstream>
#include <iostream>
#include <string>
#include <random>

// Function to generate a single random string of given length
std::string generateRandomString(size_t length) {
    const std::string charset = 
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_int_distribution<> dist(0, charset.size() - 1);

    std::string result;
    result.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        result += charset[dist(rng)];
    }

    return result;
}

int main() {
    size_t numStrings, stringLength;

    //std::cout << "Enter number of strings to generate: ";
    //std::cin >> numStrings;
    numStrings = 100000;

    std::cout << "Enter length of each string: ";
    std::cin >> stringLength;

    std::cout << "\nGenerated Random Strings:\n";
    
    std::ofstream out ("data/"+std::to_string(stringLength)+".txt");

    for (size_t i = 0; i < numStrings; ++i) {
        std::cout << generateRandomString(stringLength) << "\n";
        out << generateRandomString(stringLength) << std::endl;
    }

    return 0;
}
