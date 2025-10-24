#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <string>
#include <cstdlib>

using namespace std;

struct user {
    string name;
    int balance;
    string hash;
};

struct transaction {
    string sender_hash;
    string receiver_hash;
    int amount;
    string hash;
};

int test();

int returnVal(char x);

string hashing(string s);

int random_int(int min, int max);

string get_name();

int get_balance();

vector<transaction>  validate_transactions(vector<user> users, vector<transaction> transactions);