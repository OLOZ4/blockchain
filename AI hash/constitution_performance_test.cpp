#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <cstdint>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;
using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;

// Include hash function (same as in custom_hash.cpp)
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

void test_constitution_performance() {
    cout << "=== KONSTITUCIJOS NAŠUMO TESTAS ===\n\n";
    
    // Read full constitution
    ifstream file("konstitucija.txt");
    if (!file.is_open()) {
        cout << "Klaida: Nepavyko atidaryti konstitucijos failo!\n";
        return;
    }
    
    string constitution_content;
    string line;
    while (getline(file, line)) {
        constitution_content += line + "\n";
    }
    file.close();
    
    cout << "Konstitucijos failo dydis: " << constitution_content.length() << " simbolių\n\n";
    
    // Test with different portions of the constitution
    vector<double> portion_sizes = {0.25, 0.5, 0.75, 1.0, 2.0, 4.0, 8.0};
    
    for (double portion : portion_sizes) {
        string test_content;
        
        if (portion <= 1.0) {
            // Use portion of constitution
            size_t use_length = (size_t)(constitution_content.length() * portion);
            test_content = constitution_content.substr(0, use_length);
        } else {
            // Repeat constitution multiple times
            test_content = constitution_content;
            for (int i = 1; i < (int)portion; i++) {
                test_content += constitution_content;
            }
        }
        
        // Measure performance
        const int runs = 5;
        double total_time = 0;
        string hash_result;
        
        for (int run = 0; run < runs; run++) {
            auto start = chrono::high_resolution_clock::now();
            hash_result = custom_hash_256(test_content);
            auto end = chrono::high_resolution_clock::now();
            
            auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
            total_time += duration.count();
        }
        
        double avg_time = total_time / runs;
        double mb_per_sec = (test_content.length() / 1024.0 / 1024.0) / (avg_time / 1000000.0);
        
        cout << "Dydis: " << test_content.length() << " simbolių (" 
             << fixed << setprecision(1) << portion << "x konstitucijos)\n";
        cout << "Vidutinis laikas: " << setprecision(2) << avg_time << " μs\n";
        cout << "Našumas: " << setprecision(2) << mb_per_sec << " MB/s\n";
        cout << "Hash: " << hash_result << "\n\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    test_constitution_performance();
    
    return 0;
}