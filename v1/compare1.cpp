#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include <iomanip>
#include <string>
#include <cmath>
#include <tuple>
#include <algorithm>


using namespace std;

const int HASH_LENGTH = 64; // pvz., SHA-256 heksais = 64 simboliai

// Funkcija: skaiciuoja hex skirtumus (procentais)
double hex_difference(const string& h1, const string& h2) {
    int diff = 0;
    for (size_t i = 0; i < h1.size(); ++i) {
        if (h1[i] != h2[i])
            diff++;
    }
    return (double)diff / h1.size() * 100.0;
}

// Funkcija: skaiciuoja bit skirtumus (procentais)
double bit_difference(const string& h1, const string& h2) {
    int diff = 0;
    for (size_t i = 0; i < h1.size(); ++i) {
        uint8_t b1 = (uint8_t)strtol(h1.substr(i, 1).c_str(), nullptr, 16);
        uint8_t b2 = (uint8_t)strtol(h2.substr(i, 1).c_str(), nullptr, 16);
        bitset<4> bits1(b1);
        bitset<4> bits2(b2);
        diff += (bits1 ^ bits2).count();
    }
    return (double)diff / (h1.size() * 4) * 100.0;
}

int main() {
    ifstream infile("clean_output/1000_seq.txt");
    if (!infile) {
        cerr << "Nepavyko atidaryti failo!" << endl;
        return 1;
    }

    vector<string> hashes;
    string line;

    // Nuskaityti hash'us
    while (getline(infile, line)) {
        if (line.length() >= HASH_LENGTH)
            hashes.push_back(line.substr(0, HASH_LENGTH)); // Paimame tik pirmus 64 simbolius
    }

    infile.close();

    if (hashes.size() % 2 != 0) {
        cerr << "Nelyginis hash'u kiekis! Reikia porų." << endl;
        return 1;
    }

    vector<double> hex_diffs;
    vector<double> bit_diffs;

    // Palygink poras
    for (size_t i = 0; i < hashes.size(); i += 2) {
        const string& h1 = hashes[i];
        const string& h2 = hashes[i + 1];

        double hex_diff = hex_difference(h1, h2);
        double bit_diff = bit_difference(h1, h2);

        hex_diffs.push_back(hex_diff);
        bit_diffs.push_back(bit_diff);
    }

    // Funkcija apskaičiuoti min, max, avg
    auto calc_stats = [](const vector<double>& diffs) {
        double min_val = *min_element(diffs.begin(), diffs.end());
        double max_val = *max_element(diffs.begin(), diffs.end());
        double avg_val = 0.0;
        for (double d : diffs) avg_val += d;
        avg_val /= diffs.size();
        return make_tuple(min_val, max_val, avg_val);
    };

    auto [hex_min, hex_max, hex_avg] = calc_stats(hex_diffs);
    auto [bit_min, bit_max, bit_avg] = calc_stats(bit_diffs);

    cout << fixed << setprecision(2);
    cout << "HEX skirtumai (%):\n";
    cout << "  Min: " << hex_min << "%\n";
    cout << "  Max: " << hex_max << "%\n";
    cout << "  Avg: " << hex_avg << "%\n\n";

    cout << "BIT skirtumai (%):\n";
    cout << "  Min: " << bit_min << "%\n";
    cout << "  Max: " << bit_max << "%\n";
    cout << "  Avg: " << bit_avg << "%\n";

    return 0;
}
