#include "header.h"

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
        // probability control: ~50% chance to zero-out, tweak as you like
        if (rand() % 100 < 0.0000000000001) {
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

    string actual_name = names[random_int(0, names.size())];
    names.clear();

    return actual_name;
}

int get_balance() {
    return random_int(1, 1000000);
}

vector<transaction> validate_transactions(vector<user> users, vector<transaction> transactions) {
    std::vector<transaction> valid_txs;

    for (const auto& tx : transactions) {
        auto it_sender = std::find_if(
            users.begin(), users.end(),
            [&](const user& u){ return u.hash == tx.sender_hash; });
        auto it_receiver = std::find_if(
            users.begin(), users.end(),
            [&](const user& u){ return u.hash == tx.receiver_hash; });

        if (it_sender == users.end() || it_receiver == users.end()) {
            // unknown user
            continue;
        }

        if (it_sender->balance >= tx.amount) {
            
            // apply
            it_sender->balance   -= tx.amount;
            it_receiver->balance += tx.amount;
            
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
    block_hash.nonce = 0;
    while (true) {
        block_hash.hash = hashing(block.prev_block_hash+block.timestamp+block.version+block.merkle_root_hash+to_string(block.difficulty)+to_string(block_hash.nonce));
        //cout <<block_hash.nonce <<" " << block_hash.hash << endl;
        if (block_hash.hash[0] == '0' && block_hash.hash[1] == '0' && block_hash.hash[2] == '0') {
            return block_hash;
        }
        else {
            block_hash.nonce++;
        } 
    }
    
}

void add_block(vector<block>blockchain, vector<transaction>valid_transactions) {
    time_t timestamp = time(nullptr);
    
    block new_block;
    vector<transaction> new_block_transactions;

    /*
    if (valid_transactions.size() > 100) {
        for (int i = 0; i < 100; i++) {
            int id = random_int(0, valid_transactions.size());
            //new_block_transactions.push_back(valid_transactions[id]);
            //valid_transactions.erase(valid_transactions.begin()+id);
        }
    }
    else {
        new_block_transactions = valid_transactions;
        valid_transactions.clear();
    }

    new_block.transactions = new_block_transactions;
    new_block_transactions.clear();
    */
    cout << "Blokchain size" << blockchain.size()<<endl;
    new_block.height = blockchain.size()+1;
    //new_block.prev_block_hash = blockchain[blockchain.size()-1].curr_block_hash;
    new_block.timestamp = ctime(&timestamp);
    
    //block_hash data = get_block_hash(new_block);

    //new_block.curr_block_hash = data.hash;
    //new_block.nonce = data.nonce;

    //blockchain.push_back(new_block);
}