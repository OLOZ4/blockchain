#include <fstream>
#include <iostream>
#include <string>
#include <random>

//const std::string charset = "abcdefghijklmnopqrstuvwxyz";

const std::string charset = 
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

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

std::string generateSequentialString(size_t index, size_t length) {
    std::string result(length, charset[0]);

    size_t base = charset.size();
    size_t pos = 0;

    while (index > 0 && pos < length) {
        result[pos] = charset[index % base];
        index /= base;
        ++pos;
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
    
    std::ofstream out ("data/"+std::to_string(stringLength)+"_seq.txt");

    for (size_t i = 0; i < numStrings; ++i) {
        //std::cout << generateRandomString(stringLength) << "\n";
        //out << generateRandomString(stringLength) << std::endl;

        std::cout << generateSequentialString(i,stringLength) << "\n";
        out << generateSequentialString(i,stringLength) << std::endl;
    }

    return 0;
}
