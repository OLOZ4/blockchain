#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <string>
#include <cstdlib>
#include <ctime>
// Removed inclusion of <unistd.h> as it's not used here and causes issues on Windows.

using namespace std;

struct user {
    string name;
    int balance;
    string public_key; // user's public key (identifier)
    string hash;       // legacy field (kept for compatibility)
};

struct transaction {
    string sender_hash;
    string receiver_hash;
    int amount;
    string hash; // existing tx hash
    string transaction_id; // explicit transaction id
};

struct block {
    int height = 0;
    string prev_block_hash = "";
    string curr_block_hash = ""; 
    int nonce = 0;
    string timestamp = "";
    string version = "1.0";
    string merkle_root_hash = "";
    int difficulty = 1 + 0.1 * height;

    vector<transaction> transactions;
};

struct block_hash {
    int nonce;
    string hash;
};

int test();

int returnVal(char x);

string hashing(string s);

int random_int(int min, int max);

string get_name();

int get_balance();

vector<transaction>  validate_transactions(const vector<user>& users, const vector<transaction>& transactions);

block build_genesis_block();

block_hash get_block_hash(block block);

void add_block(vector<block>& blockchain, vector<transaction>& valid_transactions, vector<user>& users);

vector<user> generate_users (int count);

vector<transaction> generate_transactions (int count, vector<user> users);
