/*
 custom_hash.cpp

 A self-contained C++ implementation of a custom 256-bit hash function.
 This is an educational, original design (not SHA-256/MD5 or their clones).

 Features implemented to meet your requirements:
 1) Input: arbitrary-length std::string
 2) Output: fixed 256-bit (64 hex chars)
 3) Deterministic
 4) Efficient (single-pass block processing with simple bitwise ops and integer math)
 5) Designed to resist trivial collisions (many mixing rounds, non-linear ops),
    but NOT cryptographically proven — don't use for real security.
 6) Avalanche effect: mixing + rotations + multiplications intended to amplify
    small input changes into large output differences.
 7) One-way (practically irreversible for casual attackers), but again not
    proven against cryptanalysis.

 How to compile:
   g++ -O2 custom_hash.cpp -o custom_hash

 How to run:
   ./custom_hash
   (the program demonstrates a few example hashes and an avalanche test)

 Notes: This algorithm is a custom design for learning / demonstration only.
 Do not use it for production cryptography or where cryptographic guarantees
 are required.
*/

#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <cstdint>
#include <algorithm>
#include <fstream>

using namespace std;
using u8 = uint8_t;
using u32 = uint32_t;
using u64 = uint64_t;

// rotate-left for 32-bit values
static inline u32 rol(u32 x, unsigned r) {
    return (x << r) | (x >> (32 - r));
}

// simple little-endian load of 4 bytes into u32
static inline u32 load32_le(const u8 *p){
    return ((u32)p[0]) | ((u32)p[1] << 8) | ((u32)p[2] << 16) | ((u32)p[3] << 24);
}

// convert u32 words to hex string
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

// count differing bits between two hex-encoded 256-bit hashes
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

// The custom hash function
string custom_hash_256(const string &input){
    // 256-bit internal state (8 x 32-bit words)
    array<u32,8> state = {
        0xA3B1C2D4u, 0x6F5E4D3Cu, 0x9AB7E1F0u, 0x1F2E3D4Cu,
        0xC13D2E1Fu, 0x7A6B5C4Du, 0xE9F8A7B6u, 0x01234567u
    };
    // some odd constant primes for mixing
    const u32 K[16] = {
        0x6A09E667u, 0xBB67AE85u, 0x3C6EF372u, 0xA54FF53Au,
        0x510E527Fu, 0x9B05688Cu, 0x1F83D9ABu, 0x5BE0CD19u,
        0x243F6A88u, 0x85A308D3u, 0x13198A2Eu, 0x03707344u,
        0xA4093822u, 0x299F31D0u, 0x082EFA98u, 0xEC4E6C89u
    };

    // --- padding ---
    vector<u8> msg(input.begin(), input.end());
    u64 bitlen = (u64)msg.size() * 8ULL;
    // append 0x80 then zeros until length % 64 == 56
    msg.push_back(0x80);
    while((msg.size() % 64) != 56) msg.push_back(0x00);
    // append 64-bit length, little-endian
    for(int i=0;i<8;i++) msg.push_back((u8)((bitlen >> (8*i)) & 0xFF));

    // process each 64-byte block
    for(size_t off=0; off<msg.size(); off += 64){
        const u8 *block = msg.data() + off;
        u32 W[16];
        for(int i=0;i<16;i++) W[i] = load32_le(block + i*4);

        // small local copy of state
        array<u32,8> v = state;

        // 12 mixing rounds per block
        for(int round=0; round<12; ++round){
            for(int i=0;i<8;i++){
                // choose a message word and constant that vary with round
                u32 m = W[(i*3 + round) & 15];
                u32 c = K[(i + round) & 15];

                // mix: xor, add, multiply and rotate
                u32 x = v[i] ^ (m + c);
                unsigned r = (unsigned)((i * 5 + round * 7 + 13) & 31);
                x = rol(x, r);
                // non-linear step: multiply by odd constant and xor shifted neighbours
                x = (u32)((u64)x * 0x9E3779B1u); // golden ratio constant multiplication

                u32 nb = v[(i + 3) & 7];
                u32 nf = v[(i + 5) & 7];
                x ^= (nb >> ((i+1)&31)) ^ (nf << ((i+2)&31));

                // final combine
                v[i] = (x + rol(v[(i+1)&7] ^ m, ((i + round) & 31))) ^ (c ^ (u32)bitlen);
            }

            // small diffusion permutation among lanes
            array<u32,8> t;
            for(int i=0;i<8;i++) t[i] = rol(v[i] + v[(i+2)&7] ^ K[(i+round)&15], ((i*3 + round)&31));
            v = t;
        }

        // feed-forward mixing into state
        for(int i=0;i<8;i++){
            state[i] ^= (v[i] + W[i%16] + K[(i+4)&15]);
            // avalanche boost: extra rotations + xor with a shifted copy
            state[i] = rol(state[i], (i*7 + 11) & 31) ^ rol(state[(i+3)&7], (i*11 + 5) & 31);
        }
    }

    // Finalization: extra mixing rounds to ensure diffusion across all blocks
    for(int extra=0; extra<24; ++extra){
        for(int i=0;i<8;i++){
            u32 x = state[i] + K[(i+extra)&15] + (u32)((u64)state[(i+5)&7] * 0x7FEDBE3u);
            x = rol(x ^ (state[(i+2)&7] >> ((i*3+extra)&31)), ((i*5 + extra*3 + 17)&31));
            state[i] ^= x + ((u32)extra * 0x01010101u);
        }
        // small permutation
        array<u32,8> tmp;
        for(int i=0;i<8;i++) tmp[i] = rol(state[(i*3+1)&7] ^ state[(i+6)&7], (i*2 + extra)&31);
        state = tmp;
    }

    return to_hex256(state);
}

int main(int argc, char* argv[]){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Check if a filename was provided as argument
    if(argc > 1){
        string filename = argv[1];
        ifstream file(filename);
        if(!file.is_open()){
            cout << "Error: Could not open file '" << filename << "'\n";
            return 1;
        }
        
        string line;
        cout << "=== File Hash Mode ===\n";
        cout << "Reading from: " << filename << "\n\n";
        
        while(getline(file, line)){
            cout << "Input: " << line << "\n";
            cout << "Hash:  " << custom_hash_256(line) << "\n\n";
        }
        file.close();
        return 0;
    }

    // Interactive mode: continuous input
    cout << "=== Custom Hash Tool ===\n";
    cout << "Enter text to hash (type 'quit' to exit):\n\n";
    
    string line;
    while(true){
        cout << "Input: ";
        if(!getline(cin, line)) break;
        if(line == "quit") break;
        
        // Show what was entered and its hash
        cout << "Text:  \"" << line << "\"\n";
        cout << "Hash:  " << custom_hash_256(line) << "\n\n";
    }

    return 0;
}
