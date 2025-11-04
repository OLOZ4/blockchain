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

// Forward declaration so inline constructors in this header can call hashing()
string hashing(string s);

class user {
private:
    string name_;
    int balance_ = 0;
    string public_key_;
    string hash_; // legacy field

public:
    user() = default;
    user(const string &name, int balance) : name_(name), balance_(balance) {}
    user(const string &name, int balance, const string &pubkey)
        : name_(name), balance_(balance), public_key_(pubkey), hash_(pubkey) {}

    // RAII-friendly default destructor
    ~user() = default;

    // Accessors
    const string& getName() const { return name_; }
    int getBalance() const { return balance_; }
    void setBalance(int b) { balance_ = b; }
    void adjustBalance(int delta) { balance_ += delta; }
    const string& getPublicKey() const { return public_key_; }
    void setPublicKey(const string &pk) { public_key_ = pk; }
    const string& getHash() const { return hash_; }
    void setHash(const string &h) { hash_ = h; }
};

class transaction {
private:
    string sender_hash_;
    string receiver_hash_;
    int amount_ = 0;
    string hash_;
    string transaction_id_;

public:
    transaction() = default;
    transaction(const string &sender, const string &receiver, int amount)
        : sender_hash_(sender), receiver_hash_(receiver), amount_(amount) {
        // compute hash/id deterministically here
        hash_ = hashing(sender_hash_ + receiver_hash_ + to_string(amount_));
        transaction_id_ = hash_;
    }
    ~transaction() = default;

    const string& getSenderHash() const { return sender_hash_; }
    const string& getReceiverHash() const { return receiver_hash_; }
    int getAmount() const { return amount_; }
    const string& getHash() const { return hash_; }
    const string& getTransactionId() const { return transaction_id_; }
    void setSenderHash(const string &s) { sender_hash_ = s; }
    void setReceiverHash(const string &r) { receiver_hash_ = r; }
    void setAmount(int a) { amount_ = a; }
    void setHash(const string &h) { hash_ = h; }
    void setTransactionId(const string &id) { transaction_id_ = id; }
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
