#include "header.h"
#include <vector>
#include <unordered_map>
#include <chrono>

int test() {
    int i = 3;
    return i;
}

int returnVal(char x) {
    if ((int)x - 87 > 0) {
        return (int)x - 87;
    }
    else return  (int)x;      
}

string hashing(string s) {
    unsigned int var = 0;
    std::vector<int> seed(64);
    //
    
    if (s == "") std::iota(seed.begin(), seed.end(), 0); 

    else {
        for (int i = 0; i < 64; ++i) {
            char c = s[i % s.length()];
            seed[i] = (static_cast<int>(c) * (i + 31)) % 256;
            //cout << seed[i];
        }
    }

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
    
    for (int i = 0; i < 3; ++i) {
        // antras skaiciukas kuo didesnis tuo greciau atsiranda trys nuliukai
        if (rand() % 100 < 50) {
            result[i] = '0';
        }
    }


    result.resize(64);
    //cout << result<<endl;
    return result;
}

int random_int(int min, int max) {
    static std::random_device rd;        // Non-deterministic seed
    static std::mt19937 gen(rd());       // Mersenne Twister RNG
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

string get_name() {
    vector<string> names;
    ifstream in ("names.txt");

    string name;

    while (in >> name) {
        names.push_back(name);
    }

    // If names file is missing or empty, return a fallback name.
    if (names.empty()) {
        names.clear();
        return string("Unknown");
    }

    int max_idx = static_cast<int>(names.size()) - 1; // inclusive max index
    string actual_name = names[random_int(0, max_idx)];
    names.clear();
    return actual_name;
}

int get_balance() {
    return random_int(1, 1000000);
}

vector<transaction> validate_transactions(const vector<user>& users, const vector<transaction>& transactions) {
    // Build an index for users by their public_key (or hash)
    std::unordered_map<std::string, size_t> user_idx;
    for (size_t i = 0; i < users.size(); ++i) user_idx[users[i].getPublicKey()] = i;

    std::vector<transaction> valid_txs;
    valid_txs.reserve(transactions.size());

    for (const auto& tx : transactions) {
        auto it_sender = user_idx.find(tx.getSenderHash());
        auto it_receiver = user_idx.find(tx.getReceiverHash());

        if (it_sender == user_idx.end() || it_receiver == user_idx.end()) {
            continue; // unknown user
        }

        const user& sender = users[it_sender->second];
        // Verify transaction_id matches recomputed hash (sender+receiver+amount)
        string expected_tx_hash = hashing(tx.getSenderHash() + tx.getReceiverHash() + to_string(tx.getAmount()));
        if (tx.getTransactionId() != expected_tx_hash) {
            continue; // invalid transaction id
        }

        // only validate here (do not mutate users); balances applied when block is created
        if (sender.getBalance() >= tx.getAmount()) {
            valid_txs.push_back(tx);
        }
    }

    return valid_txs;

}

block build_genesis_block() {
    time_t timestamp = time(nullptr);

    
    block genesis;
    genesis.height = 1;
    genesis.prev_block_hash = "0";
    genesis.timestamp = ctime(&timestamp);
    
    //block_hash gen = get_block_hash(genesis);

    block_hash gen;
    gen.hash = "000455412474814151v23214393365542W2153Z2456933065b31Em147Sv149Rs";
    gen.nonce = 61706;

    genesis.curr_block_hash = gen.hash;
    genesis.nonce = gen.nonce;

    
    

    return genesis;
}

block_hash get_block_hash(block block) {
    block_hash block_hash;
    
    block_hash.hash = "1";
    block_hash.nonce = 1;
    while (true) {
        block_hash.hash = hashing(block.prev_block_hash+block.timestamp+block.version+block.merkle_root_hash+to_string(block.difficulty)+to_string(block_hash.nonce));
        //cout <<block_hash.nonce <<" " << block_hash.hash << endl;
        if (block_hash.hash[0] == '0' && block_hash.hash[1] == '0' && block_hash.hash[2] == '0') {
            cout <<"Found a hash!:"<< block_hash.hash << " Nonce: " << block_hash.nonce << endl;
            //sleep(2);
            return block_hash;
        }
        else {
            //cout <<"Mining: "<< "Nonce"<< block_hash.nonce<<" " <<block_hash.hash<<endl;
            block_hash.nonce++;
        } 
    }
    
}

void add_block(vector<block>& blockchain, vector<transaction>& valid_transactions, vector<user>& users) {
    time_t timestamp = time(nullptr);
    
    block new_block;
    vector<transaction> new_block_transactions;

    
    // Build a quick lookup of users by public_key
    std::unordered_map<std::string, size_t> user_idx;
    for (size_t i = 0; i < users.size(); ++i) user_idx[users[i].getPublicKey()] = i;

    if (valid_transactions.size() > 100) {
        for (int i = 0; i < 100; i++) {
            int id = random_int(0, static_cast<int>(valid_transactions.size()) - 1);
            transaction tx = valid_transactions[id];

            // Apply balances when including tx in a block
            auto s = user_idx.find(tx.getSenderHash());
            auto r = user_idx.find(tx.getReceiverHash());
            if (s != user_idx.end() && r != user_idx.end()) {
                if (users[s->second].getBalance() >= tx.getAmount()) {
                    users[s->second].adjustBalance(-tx.getAmount());
                    users[r->second].adjustBalance(tx.getAmount());
                    new_block_transactions.push_back(tx);
                }
            }

            valid_transactions.erase(valid_transactions.begin()+id);
        }
    }
    else {
        // include remaining valid transactions and apply balances
        for (const auto &tx : valid_transactions) {
            auto s = user_idx.find(tx.getSenderHash());
            auto r = user_idx.find(tx.getReceiverHash());
            if (s != user_idx.end() && r != user_idx.end()) {
                if (users[s->second].getBalance() >= tx.getAmount()) {
                    users[s->second].adjustBalance(-tx.getAmount());
                    users[r->second].adjustBalance(tx.getAmount());
                    new_block_transactions.push_back(tx);
                }
            }
        }
        valid_transactions.clear();
    }

    new_block.transactions = new_block_transactions;
    // Compute a simple Merkle-like root: concatenated tx ids hashed
    if (!new_block.transactions.empty()) {
        string concat_ids = "";
        for (const auto &tx : new_block.transactions) concat_ids += tx.getTransactionId();
        new_block.merkle_root_hash = hashing(concat_ids);
    } else {
        new_block.merkle_root_hash = "";
    }
    new_block_transactions.clear();
    
    //cout << "Blokchain size" << blockchain.size()<<endl;
    new_block.height = blockchain.size()+1;
    new_block.prev_block_hash = blockchain[blockchain.size()-1].curr_block_hash;
    new_block.timestamp = ctime(&timestamp);
    
    block_hash data = get_block_hash(new_block);

    new_block.curr_block_hash = data.hash;
    new_block.nonce = data.nonce;

    // Print a concise block summary
    cout << "Block #" << new_block.height << " mined: nonce=" << new_block.nonce
        << " hash=" << new_block.curr_block_hash << " merkle=" << new_block.merkle_root_hash
        << " tx_count=" << new_block.transactions.size() << endl;

    blockchain.push_back(new_block);
}

vector<user> generate_users (int count) {
    vector<user> users;

    for (int i = 0; i < count; i++){
        user u(get_name(), get_balance());
        // generate a public_key and keep legacy hash in sync
        string pub = hashing(u.getName() + to_string(u.getBalance()) + to_string(rand()));
        u.setPublicKey(pub);
        u.setHash(pub);
        users.push_back(std::move(u));

        //cout <<users[i].name<< " "<< users[i].balance << " " << users[i].hash << endl;
    }
    cout << "Created " << count << " users"<<endl;
    return users;
}

vector<transaction> generate_transactions (int count, vector<user> users) {
    vector<transaction> transactions;
    for (int i = 0; i < count; i++) {
        // pick sender and receiver indices
        int sender_idx = random_int(0, static_cast<int>(users.size()) - 1);
        int receiver_idx = random_int(0, static_cast<int>(users.size()) - 1);
        // avoid self-transfer
        while (receiver_idx == sender_idx && users.size() > 1) {
            receiver_idx = random_int(0, static_cast<int>(users.size()) - 1);
        }

        int max_amt = users[sender_idx].getBalance();
        if (max_amt < 1) max_amt = 1;
        int amt = random_int(1, max_amt);

        const string &s = users[sender_idx].getPublicKey();
        const string &r = users[receiver_idx].getPublicKey();
        transaction t(s, r, amt);
        transactions.push_back(std::move(t));

        //cout <<"["<<i<<"] " << transactions[i].sender_hash << " " << transactions[i].receiver_hash << " " << transactions[i].amount << endl;
        //cout <<"["<<i<<"] " << transactions[i].hash << " " << transactions[i].amount << endl;

    }
    cout << "Created " << count << " transactions"<<endl;

    return transactions;

}
