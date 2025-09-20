/*
Pseudo kodas

Tarkime kad yra kazokie skaicai kaip seed'as

Ivedame kazkoki teskta

teksta pagal tam tikra algoritma pakeiciame i skaicius

skaicius sudedame ir isrusiuojame

gauname kazkoki unikalu skaiciu is ivesties

ji naudojame kaip kazkoki unikalu rakta, kuris padaro hasha unikalu


*/


#include <algorithm>
#include <cmath>
#include <cstring>
#include <iomanip>
using namespace std;
#include <string>
#include <iostream>
#include <vector>

int returnVal(char x);

string hashing(string s); 
/*
00000062543822610264258D17115314662NVF54382379210615438226102642 - "test"

0004932137H5233600004223524543515N317024514C45263N2452H623150311 - "test1"

0062212121181817570062212121887CC063111111887CCN09191921NMM32219 - "test2"

*/

int main() {

    cout << hashing("test") << endl;
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

    string result = "";

    for (auto i : seed) {

        //cout << seed[i] << " -> ";
        if (seed[i] > 65 && seed[i] < 122) {
            //cout << char(seed[i]);
            result += char(seed[i]);
        }
        else {
            //cout <<seed[i];
            result += to_string((seed[i]));
        }
        //cout << endl;
    }

    cout <<  endl << result.length() << endl;

    /*
    for (int i = 0; i < 64; i++) {
        cout << result[i];
    }
    */

    result.resize(64);
    return result;

}