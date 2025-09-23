#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

// Include the hash function (simplified version)
extern string custom_hash_256(const string &input);

void demonstrate_avalanche() {
    cout << "=== LAVINOS EFEKTO DEMONSTRACIJA ===\n\n";
    
    vector<pair<string, string>> test_pairs = {
        {"Hello", "hello"},                    // Case change
        {"test", "Test"},                      // Case change  
        {"password", "password1"},             // Add character
        {"algorithm", "algoritm"},             // Remove character
        {"12345", "12346"},                   // Last digit change
        {"The quick brown fox", "The quick brown fax"}, // Middle change
    };
    
    for(const auto& pair : test_pairs) {
        string hash1 = custom_hash_256(pair.first);
        string hash2 = custom_hash_256(pair.second);
        
        // Count different characters
        int diff_count = 0;
        for(size_t i = 0; i < hash1.length() && i < hash2.length(); i++) {
            if(hash1[i] != hash2[i]) diff_count++;
        }
        
        double diff_percent = (double)diff_count / hash1.length() * 100;
        
        cout << "Input 1: \"" << pair.first << "\"\n";
        cout << "Input 2: \"" << pair.second << "\"\n";
        cout << "Hash 1:  " << hash1 << "\n";
        cout << "Hash 2:  " << hash2 << "\n";
        cout << "Difference: " << diff_count << "/" << hash1.length() 
             << " characters (" << fixed << setprecision(1) << diff_percent << "%)\n";
        cout << "Visual: ";
        
        // Visual representation
        for(size_t i = 0; i < hash1.length() && i < hash2.length(); i++) {
            if(hash1[i] != hash2[i]) {
                cout << "■";  // Different
            } else {
                cout << "□";  // Same
            }
        }
        cout << "\n\n";
    }
}

int main() {
    demonstrate_avalanche();
    return 0;
}