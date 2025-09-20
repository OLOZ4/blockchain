#include <algorithm>
#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int returnVal(char x);

string hashing(string s); 

string read_file(const std::string& filename);

void read_line(const std::string& filename);

/*
ZVRNJFB54382261026425868171153146627886705438237921D615438226102 - "test"

1767992352NE493213772523360XOF422352454351578317H245146745263782 - "test1"

1870029171MC21212118181757WMC2121218876767ND111111887676778E9191 - "test2"

*/

int main() {
    
    //string text = read_file("data/konstitucija.txt");
    read_line("data/single.txt");
    //cout << hashing("") << endl;
    return 0;
}

int returnVal(char x) {
    return (int)x - 87;
}

string hashing(string s) {
    vector<int> seed;
    //string s = "test2";
    unsigned int var = 0;

    for (int i = 0; i < 64; i++) {
        seed.push_back(i);
        //cout <<seed[i]<<endl;
        
    }

    for (unsigned int i = 0; i < s.length(); i++)
    {
        //cout << returnVal(s[i]) << " ";
        seed.push_back(returnVal(s[i]));
        var += returnVal(s[i]) *  pow(10,i+1);
    }

    //cout << endl << var << endl;

    stable_sort(seed.begin(), seed.end());
    

    int sum = 0;

    for (auto i : seed) {
        //cout << i << " ";
        if (sum < 1000) sum +=i;
    }

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

        //cout << seed[i] << " -> ";
        if (i > 65 && i < 122) {
            //cout << char(seed[i]);
            result += static_cast<char>(i);
        }
        else {
            //cout <<seed[i];
            result += to_string((seed[i]));
        }
        //cout << endl;
    }

    //cout <<  endl << result.length() << endl;

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
    std::ifstream file(filename);

    if (!file) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        cout << line << " -> " << hashing(line) << endl;
    }
}