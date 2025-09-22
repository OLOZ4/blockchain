#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric>

using namespace std;

int returnVal(char x);

string hashing(string s); 

string read_file(const std::string& filename);

void read_line(const std::string& filename);

string hashing1(string s);

int returnVal3(char x);

std::string hashing3(const std::string& s);

/*
ZVRNJFB54382261026425868171153146627886705438237921D615438226102 - "test"

1767992352NE493213772523360XOF422352454351578317H245146745263782 - "test1"

1870029171MC21212118181757WMC2121218876767ND111111887676778E9191 - "test2"

*/

int main() {
    
    //string text = read_file("data/konstitucija.txt");
    //read_line("1000.txt");
    
    cout << hashing1("") << endl;
    //cout << hashing1("Test") << endl;
    //cout << hashing("test") << endl;
    //cout << hashing("Test") << endl;
    //cout << hashing("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010000000000000000000000000000000000000000000000");
    return 0;
}

int returnVal(char x) {
    if ((int)x - 87 > 0) {
        return (int)x - 87;
    }
    else return  (int)x;      
}

int returnVal1(char x) {
    return static_cast<int>(x);
}

string hashing(string s) {
    //vector<int> seed;
    //string s = "test2";
    unsigned int var = 0;

    std::vector<int> seed(64);
    std::iota(seed.begin(), seed.end(), 0); 

    for (unsigned int i = 0; i < s.length(); i++)
    {
        //cout << returnVal(s[i]) << " ";
        seed.push_back(returnVal(s[i]));
        var += returnVal(s[i]) *  pow(10,i+1);
    }

    //cout <<"var: " << var << endl;

    stable_sort(seed.begin(), seed.end());
    

    int sum = 0;

    for (auto i : seed) {
        //cout << i << " ";
        if (sum < 1000) sum +=i;
    }

    //abs(sum);

    //cout << "sum: " << sum << endl;

    int var2;
    for (int i = 0; i < 64; i++) {
        var2 = (var - sum*i)%100;
        //cout << var2<< endl;
        seed[i]=abs(seed[i]-var2); 
    }

    //cout <<"-------------------"<<endl;

    string result= "";

    for (auto i : seed) {

        //cout << i << " ";
        if (i > 65 && i < 91 || i > 96 && i < 123) {
            //cout << char(seed[i]);
            result += static_cast<char>(i);
        }

        else {
            //cout <<seed[i];
            result += to_string(i);
        }
        //cout << endl;
    }

    //cout <<  endl << result.length() << endl;

    result.resize(64);
    return result;

}

string hashing1(string s) {
    unsigned int var = 0;
    std::vector<int> seed(64);
    std::iota(seed.begin(), seed.end(), 0); 

    for (unsigned int i = 0; i < s.length(); i++) {
        seed[i % 64] ^= returnVal(s[i]) + i;
    }

    std::stable_sort(seed.begin(), seed.end());

    int sum = 0;
    for (auto i : seed) {
        if (sum < 1000) sum += i;
    }

    for (int i = 0; i < 64; i++) {
        int var2 = (var - sum * i) % 100;
        seed[i] = abs(seed[i] - var2);
    }

    for (unsigned int i = 0; i < s.length(); i++) {
    seed[i % 64] ^= returnVal(s[i]) + i;  
    }

    string result = "";
    for (auto val : seed) {
        if ((val > 65 && val < 91) || (val > 96 && val < 123)) {
            result += static_cast<char>(val);
        } else {
            result += to_string(val);
        }
    }

    result.resize(64);
    return result;
}

string read_file(const std::string& filename) {
    std::ifstream file(filename); 

    if (!file) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::ostringstream ss;
    ss << file.rdbuf();

    return ss.str();
}

void read_line(const std::string& filename) {
    std::ifstream file("data/"+filename);
    std::ofstream out ("clean_output/"+filename);

    if (!file) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        //cout << line << " -> " << hashing(line) << endl;
        //out << line << " -> " << hashing(line) << endl;
        out << hashing1(line) << endl;
    }
}