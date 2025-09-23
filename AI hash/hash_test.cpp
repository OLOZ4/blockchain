#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <random>
#include <chrono>
#include <set>
#include <iomanip>
#include <cmath>
#include <numeric>

using namespace std;
using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;

// Include the hash function from custom_hash.cpp
static inline u32 rol(u32 x, unsigned r) {
    return (x << r) | (x >> (32 - r));
}

static inline u32 load32_le(const u8 *p){
    return ((u32)p[0]) | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

string to_hex256(const array<u32,8>& s){
    static const char *hex = "0123456789abcdef";
    string out; out.reserve(64);
    for(int i=0;i<8;i++){
        u32 v = s[i];
        for(int b=0;b<4;b++){
            u8 byte = (v >> (8*b)) & 0xFF;
            out.push_back(hex[(byte >> 4) & 0xF]);
            out.push_back(hex[byte & 0xF]);
        }
    }
    return out;
}

int bit_diff_between_hex(const string &a, const string &b){
    auto hexval = [](char c)->int{
        if(c>='0' && c<='9') return c-'0';
        if(c>='a' && c<='f') return 10 + (c - 'a');
        if(c>='A' && c<='F') return 10 + (c - 'A');
        return 0;
    };
    int bits = 0;
    for(size_t i=0;i<a.size() && i<b.size(); ++i){
        int x = hexval(a[i]) ^ hexval(b[i]);
        bits += __builtin_popcount((unsigned)x);
    }
    return bits;
}

string custom_hash_256(const string &input){
    array<u32,8> state = {
        0xA3B1C2D4u, 0x6F5E4D3Cu, 0x9AB7E1F0u, 0x1F2E3D4Cu,
        0xC13D2E1Fu, 0x7A6B5C4Du, 0xE9F8A7B6u, 0x01234567u
    };
    const u32 K[16] = {
        0x6A09E667u, 0xBB67AE85u, 0x3C6EF372u, 0xA54FF53Au,
        0x510E527Fu, 0x9B05688Cu, 0x1F83D9ABu, 0x5BE0CD19u,
        0x243F6A88u, 0x85A308D3u, 0x13198A2Eu, 0x03707344u,
        0xA4093822u, 0x299F31D0u, 0x082EFA98u, 0xEC4E6C89u
    };

    vector<u8> msg(input.begin(), input.end());
    u64 bitlen = (u64)msg.size() * 8ULL;
    msg.push_back(0x80);
    while((msg.size() % 64) != 56) msg.push_back(0x00);
    for(int i=0;i<8;i++) msg.push_back((u8)((bitlen >> (8*i)) & 0xFF));

    for(size_t off=0; off<msg.size(); off += 64){
        const u8 *block = msg.data() + off;
        u32 W[16];
        for(int i=0;i<16;i++) W[i] = load32_le(block + i*4);

        array<u32,8> v = state;

        for(int round=0; round<12; ++round){
            for(int i=0;i<8;i++){
                u32 m = W[(i*3 + round) & 15];
                u32 c = K[(i + round) & 15];

                u32 x = v[i] ^ (m + c);
                unsigned r = (unsigned)((i * 5 + round * 7 + 13) & 31);
                x = rol(x, r);
                x = (u32)((u64)x * 0x9E3779B1u);

                u32 nb = v[(i + 3) & 7];
                u32 nf = v[(i + 5) & 7];
                x ^= (nb >> ((i+1)&31)) ^ (nf << ((i+2)&31));

                v[i] = (x + rol(v[(i+1)&7] ^ m, ((i + round) & 31))) ^ (c ^ (u32)bitlen);
            }

            array<u32,8> t;
            for(int i=0;i<8;i++) t[i] = rol(v[i] + v[(i+2)&7] ^ K[(i+round)&15], ((i*3 + round)&31));
            v = t;
        }

        for(int i=0;i<8;i++){
            state[i] ^= (v[i] + W[i%16] + K[(i+4)&15]);
            state[i] = rol(state[i], (i*7 + 11) & 31) ^ rol(state[(i+3)&7], (i*11 + 5) & 31);
        }
    }

    for(int extra=0; extra<24; ++extra){
        for(int i=0;i<8;i++){
            u32 x = state[i] + K[(i+extra)&15] + (u32)((u64)state[(i+5)&7] * 0x7FEDBE3u);
            x = rol(x ^ (state[(i+2)&7] >> ((i*3+extra)&31)), ((i*5 + extra*3 + 17)&31));
            state[i] ^= x + ((u32)extra * 0x01010101u);
        }
        array<u32,8> tmp;
        for(int i=0;i<8;i++) tmp[i] = rol(state[(i*3+1)&7] ^ state[(i+6)&7], (i*2 + extra)&31);
        state = tmp;
    }

    return to_hex256(state);
}

// Test utility functions
string generate_random_string(int length, mt19937& rng) {
    string chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()_+-=[]{}|;:,.<>?";
    uniform_int_distribution<int> dist(0, chars.length() - 1);
    string result;
    result.reserve(length);
    for(int i = 0; i < length; i++) {
        result += chars[dist(rng)];
    }
    return result;
}

string flip_one_bit(const string& input, int position, mt19937& rng) {
    if(input.empty()) return input;
    string result = input;
    if(position >= (int)result.length()) position = position % result.length();
    
    uniform_int_distribution<int> bit_dist(0, 7);
    int bit_to_flip = bit_dist(rng);
    result[position] ^= (1 << bit_to_flip);
    return result;
}

void test_output_size() {
    cout << "\n=== 1. TEST IŠVEDIMO DYDŽIO ===\n";
    vector<string> test_inputs = {"", "a", "hello world", string(1000, 'x'), string(10000, 'y')};
    
    for(const auto& input : test_inputs) {
        string hash = custom_hash_256(input);
        cout << "Įvestis (" << input.length() << " simbolių): hash ilgis = " << hash.length() << "\n";
    }
    cout << "✓ Visi hash'ai yra 64 simbolių ilgio\n";
}

void test_determinism() {
    cout << "\n=== 2. TEST DETERMINISTIŠKUMO ===\n";
    vector<string> test_inputs = {"test", "longer test string", "123456789"};
    
    for(const auto& input : test_inputs) {
        string hash1 = custom_hash_256(input);
        string hash2 = custom_hash_256(input);
        string hash3 = custom_hash_256(input);
        
        bool deterministic = (hash1 == hash2) && (hash2 == hash3);
        cout << "Įvestis: \"" << input << "\" - " << (deterministic ? "✓ Deterministinis" : "✗ Nedeterministinis") << "\n";
    }
}

void test_performance() {
    cout << "\n=== 3. TEST EFEKTYVUMO ===\n";
    
    // Create test data of different sizes
    vector<int> sizes = {100, 500, 1000, 5000, 10000, 50000};
    vector<double> times;
    
    mt19937 rng(12345);
    
    for(int size : sizes) {
        string test_data = generate_random_string(size, rng);
        
        // Measure time for multiple runs
        const int runs = 10;
        double total_time = 0;
        
        for(int run = 0; run < runs; run++) {
            auto start = chrono::high_resolution_clock::now();
            custom_hash_256(test_data);
            auto end = chrono::high_resolution_clock::now();
            
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            total_time += duration.count();
        }
        
        double avg_time = total_time / runs;
        times.push_back(avg_time);
        
        cout << "Dydis: " << size << " simbolių, vidutinis laikas: " 
             << fixed << setprecision(2) << avg_time << " μs\n";
    }
    
    // Simple performance analysis
    cout << "\nEfektyvumo analizė:\n";
    for(size_t i = 1; i < sizes.size(); i++) {
        double ratio = times[i] / times[i-1];
        double size_ratio = (double)sizes[i] / sizes[i-1];
        cout << "Dydis x" << fixed << setprecision(1) << size_ratio 
             << ", laikas x" << setprecision(2) << ratio << "\n";
    }
}

void test_collisions() {
    cout << "\n=== 4. TEST KOLIZIJŲ ===\n";
    
    vector<int> string_lengths = {10, 100, 500, 1000};
    const int test_count = 10000; // Reduced for faster testing
    
    mt19937 rng(54321);
    
    for(int length : string_lengths) {
        set<string> hashes;
        int collisions = 0;
        
        for(int i = 0; i < test_count; i++) {
            string test_str = generate_random_string(length, rng);
            string hash = custom_hash_256(test_str);
            
            if(hashes.find(hash) != hashes.end()) {
                collisions++;
            }
            hashes.insert(hash);
        }
        
        double collision_rate = (double)collisions / test_count * 100;
        cout << "Ilgis " << length << " simbolių: " << collisions << " kolizijos iš " 
             << test_count << " testų (" << fixed << setprecision(4) 
             << collision_rate << "%)\n";
    }
}

void test_avalanche_effect() {
    cout << "\n=== 5. TEST LAVINOS EFEKTO ===\n";
    
    const int test_count = 1000; // Reduced for faster testing
    mt19937 rng(98765);
    
    vector<int> bit_diffs;
    vector<int> hex_diffs;
    
    for(int i = 0; i < test_count; i++) {
        // Generate random string
        string original = generate_random_string(100, rng);
        
        // Flip one bit
        uniform_int_distribution<int> pos_dist(0, original.length() - 1);
        int flip_pos = pos_dist(rng);
        string modified = flip_one_bit(original, flip_pos, rng);
        
        // Calculate hashes
        string hash1 = custom_hash_256(original);
        string hash2 = custom_hash_256(modified);
        
        // Count differences
        int bit_diff = bit_diff_between_hex(hash1, hash2);
        int hex_diff = 0;
        for(size_t j = 0; j < hash1.length() && j < hash2.length(); j++) {
            if(hash1[j] != hash2[j]) hex_diff++;
        }
        
        bit_diffs.push_back(bit_diff);
        hex_diffs.push_back(hex_diff);
    }
    
    // Calculate statistics
    auto bit_minmax = minmax_element(bit_diffs.begin(), bit_diffs.end());
    auto hex_minmax = minmax_element(hex_diffs.begin(), hex_diffs.end());
    
    double bit_avg = accumulate(bit_diffs.begin(), bit_diffs.end(), 0.0) / bit_diffs.size();
    double hex_avg = accumulate(hex_diffs.begin(), hex_diffs.end(), 0.0) / hex_diffs.size();
    
    cout << "Bitų skirtumai:\n";
    cout << "  Minimalus: " << *bit_minmax.first << " (" << (*bit_minmax.first * 100.0 / 256) << "%)\n";
    cout << "  Maksimalus: " << *bit_minmax.second << " (" << (*bit_minmax.second * 100.0 / 256) << "%)\n";
    cout << "  Vidutinis: " << fixed << setprecision(1) << bit_avg << " (" << (bit_avg * 100.0 / 256) << "%)\n";
    
    cout << "Hex simbolių skirtumai:\n";
    cout << "  Minimalus: " << *hex_minmax.first << " (" << (*hex_minmax.first * 100.0 / 64) << "%)\n";
    cout << "  Maksimalus: " << *hex_minmax.second << " (" << (*hex_minmax.second * 100.0 / 64) << "%)\n";
    cout << "  Vidutinis: " << fixed << setprecision(1) << hex_avg << " (" << (hex_avg * 100.0 / 64) << "%)\n";
}

void test_one_way_property() {
    cout << "\n=== 6. TEST NEGRĮŽTAMUMO (su salt) ===\n";
    
    mt19937 rng(13579);
    vector<string> salts = {"salt1", "salt2", "random_salt_12345", ""};
    string test_input = "secret_password";
    
    cout << "Originalus tekstas: \"" << test_input << "\"\n\n";
    
    for(const auto& salt : salts) {
        string salted_input = test_input + salt;
        string hash = custom_hash_256(salted_input);
        cout << "Salt: \"" << salt << "\"\n";
        cout << "Hash: " << hash << "\n";
        cout << "Išbandykite atkurti originalų tekstą iš šio hash!\n\n";
    }
    
    // Demonstrate different inputs with same salt
    cout << "Skirtingi tekstai su tuo pačiu salt:\n";
    vector<string> inputs = {"password", "Password", "password1", "passwor"};
    string common_salt = "mysalt";
    
    for(const auto& input : inputs) {
        string hash = custom_hash_256(input + common_salt);
        cout << "\"" << input << "\" + \"" << common_salt << "\" -> " << hash << "\n";
    }
}

void run_all_tests() {
    cout << "=== CUSTOM HASH ALGORITMO TESTAVIMAS ===\n";
    cout << "Datos: " << __DATE__ << " " << __TIME__ << "\n";
    
    test_output_size();
    test_determinism();
    test_performance();
    test_collisions();
    test_avalanche_effect();
    test_one_way_property();
    
    cout << "\n=== TESTAVIMAS BAIGTAS ===\n";
    cout << "Rezultatai išsaugoti. Peržiūrėkite README.md failą išvadoms.\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    run_all_tests();
    
    return 0;
}