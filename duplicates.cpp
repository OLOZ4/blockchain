#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>

int main() {
    std::string filename;
    std::cout << "Enter the filename: ";
    std::getline(std::cin, filename);

    std::ifstream infile(filename);
    if (!infile) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return 1;
    }

    std::unordered_map<std::string, int> lineCount;
    std::string line;
    while (std::getline(infile, line)) {
        ++lineCount[line];
    }

    infile.close();

    int duplicateLines = 0;
    for (const auto& pair : lineCount) {
        if (pair.second > 1) {
            ++duplicateLines;
            //std::cout << "Line: \"" << pair.first << "\" appears " << pair.second << " times." << std::endl;
        }
    }

    std::cout << "\nTotal number of **identical (repeated)** lines: " << duplicateLines << std::endl;

    return 0;
}
