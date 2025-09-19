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

int returnVal(char x) {
    return (int)x - 87;
}

int main() {
    vector<int> seed;
    string s = "ą";
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

    cout << endl << var << endl;

    stable_sort(seed.begin(), seed.end());
    

    int sum = 0;

    for (auto i : seed) {
        //cout << i << " ";
        if (sum < 1000) sum +=i;
    }

    cout << "sum: " << sum << endl;

    /*
    float var2 = var * 1.0;
    for (int i = 0; i < 5; i++){
        var2 = var2/2.0;
        cout << "var2: "<< (int(var2*100))/((to_string(var2).length())-10)<< " " << var2 << endl;
        
        
    }
    */
    int var2;
    for (int i = 0; i < 64; i++) {
        var2 = (var - sum*i)%100;
        //cout << var2<< endl;
        seed[i]=abs(seed[i]-var2); 
    }


    return 0;
}