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
    

    // deterministic zero injection so hashing(s) is stable across calls
    size_t hval = std::hash<string>{}(s);
    for (int i = 0; i < 3; ++i) {
        // use different bits of hval to get per-position variability
    if (rand() % 100 < diff) {
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
    // Perform light pre-inclusion validation:
    // - ensure sender and receiver exist
    // - ensure the transaction id/hash matches recomputed value
    // Do NOT check balances here; balances are dynamic and are checked when
    // selecting transactions for a block (in add_block()).

    std::unordered_map<std::string, size_t> user_idx;
    for (size_t i = 0; i < users.size(); ++i) user_idx[users[i].getPublicKey()] = i;

    vector<transaction> out;
    out.reserve(transactions.size());
    std::unordered_set<std::string> seen_ids;

    // Build a mutable view of balances to perform balance checks during
    // pre-validation. This accepts transactions up to the available balance
    // when iterating in order. It prevents trivially invalid transactions
    // (sender can't cover cumulative outgoing amount observed here).
    std::unordered_map<std::string, long long> avail_balance;
    avail_balance.reserve(users.size());
    for (const auto &u : users) avail_balance[u.getPublicKey()] = u.getBalance();

    for (const auto &tx : transactions) {
        // reject obvious invalids early
        if (tx.getAmount() <= 0) continue;                    // non-positive amount
        if (tx.getSenderHash() == tx.getReceiverHash()) continue; // self-transfer

        auto s = user_idx.find(tx.getSenderHash());
        auto r = user_idx.find(tx.getReceiverHash());
        if (s == user_idx.end() || r == user_idx.end()) {
            // unknown participant, drop the transaction
            continue;
        }

        // recompute id/hash deterministically and compare
        string expected = hashing(tx.getSenderHash() + tx.getReceiverHash() + to_string(tx.getAmount()));
        if (expected != tx.getTransactionId()) {
            // malformed or tampered transaction, drop it
            continue;
        }

        // reject duplicate transaction ids (keep first occurrence)
        if (seen_ids.find(tx.getTransactionId()) != seen_ids.end()) continue;
        // check sender balance against our running available balance
        auto bal_it = avail_balance.find(tx.getSenderHash());
        if (bal_it == avail_balance.end()) continue; // should not happen, defensive
        if (bal_it->second < tx.getAmount()) {
            // sender cannot cover this tx given previously accepted ones
            continue;
        }

        // accept tx: reserve amount
        bal_it->second -= tx.getAmount();
        seen_ids.insert(tx.getTransactionId());
        out.push_back(tx);
    }

    return out;

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
            //cout <<"Found a hash!:"<< block_hash.hash << " Nonce: " << block_hash.nonce << endl;
            //sleep(2);
            return block_hash;
        }
        else {
            //cout <<"Mining: "<< "Nonce"<< block_hash.nonce<<" " <<block_hash.hash<<endl;
            block_hash.nonce++;
        } 
    }
    
}

block_hash get_block_hash_threaded(const block& block) {
    //const int num_threads = 2;
    //unsigned int num_threads = thread::hardware_concurrency();

    atomic<bool> found(false);
    block_hash result;
    mutex result_mutex;

    auto worker = [&](int thread_id) {
        block_hash local_hash;
        local_hash.nonce = thread_id;  // each thread starts at a different nonce

        while (!found) {
            local_hash.hash = hashing(
                block.prev_block_hash +
                block.timestamp +
                block.version +
                block.merkle_root_hash +
                to_string(block.difficulty) +
                to_string(local_hash.nonce)
            );

            // check if hash meets difficulty (3 leading '0's here)
            if (local_hash.hash[0] == '0' && local_hash.hash[1] == '0' && local_hash.hash[2] == '0') {
                // Save result only once
                {
                    lock_guard<mutex> lock(result_mutex);
                    if (!found) {
                        local_hash.core = thread_id +1;
                        found = true;
                        result = local_hash;
                        
                        //cout <<endl<<"(Core " << thread_id+1 << "): ";
                    }
                }
                break;
            }

            // increment nonce differently for each thread to avoid overlap
            local_hash.nonce += num_threads;
        }
    };

    vector<thread> threads;
    for (int i = 0; i < num_threads; ++i)
        threads.emplace_back(worker, i);

    for (auto& t : threads) t.join();

    return result;
}

void add_block(vector<block>& blockchain, vector<transaction>& valid_transactions, vector<user>& users) {
    time_t timestamp = time(nullptr);
    
    block new_block;
    vector<transaction> new_block_transactions;

    
    // Build a quick lookup of users by public_key
    std::unordered_map<std::string, size_t> user_idx;
    for (size_t i = 0; i < users.size(); ++i) user_idx[users[i].getPublicKey()] = i;
    // Select up to 100 transactions to include in the block while preserving
    // skipped-but-valid transactions in the mempool.
    if (!valid_transactions.empty()) {
        // Shuffle mempool to avoid bias
        std::mt19937 rng(std::random_device{}());
        std::shuffle(valid_transactions.begin(), valid_transactions.end(), rng);

        vector<transaction> remaining;
        remaining.reserve(valid_transactions.size());

        for (const auto &tx : valid_transactions) {
            if (new_block_transactions.size() >= 100) {
                // keep remaining txs for future blocks
                remaining.push_back(tx);
                continue;
            }

            auto s = user_idx.find(tx.getSenderHash());
            auto r = user_idx.find(tx.getReceiverHash());
            if (s == user_idx.end() || r == user_idx.end()) {
                // malformed/unknown participants should have been filtered earlier
                // but skip them defensively
                continue;
            }

            if (users[s->second].getBalance() >= tx.getAmount()) {
                // apply balances and include tx
                users[s->second].adjustBalance(-tx.getAmount());
                users[r->second].adjustBalance(tx.getAmount());
                new_block_transactions.push_back(tx);
            } else {
                // sender has insufficient funds right now: keep tx in mempool
                remaining.push_back(tx);
            }
        }

        // If nothing could be included from the shuffled pass, try a fallback
        // that prefers smallest transactions (better packing). This often
        // progresses the mempool when shuffle missed many small txs.
        if (new_block_transactions.empty()) {
            // create a copy sorted by amount ascending
            vector<transaction> sorted = valid_transactions;
            std::sort(sorted.begin(), sorted.end(), [](const transaction &a, const transaction &b){
                return a.getAmount() < b.getAmount();
            });

            // try to include up to 100 smallest txs
            std::vector<std::string> included_ids;
            included_ids.reserve(100);
            for (const auto &tx : sorted) {
                if (new_block_transactions.size() >= 100) break;
                auto s = user_idx.find(tx.getSenderHash());
                auto r = user_idx.find(tx.getReceiverHash());
                if (s == user_idx.end() || r == user_idx.end()) continue;
                if (users[s->second].getBalance() >= tx.getAmount()) {
                    users[s->second].adjustBalance(-tx.getAmount());
                    users[r->second].adjustBalance(tx.getAmount());
                    new_block_transactions.push_back(tx);
                    included_ids.push_back(tx.getTransactionId());
                }
            }

            if (!new_block_transactions.empty()) {
                // rebuild remaining excluding included txs
                vector<transaction> remaining2;
                remaining2.reserve(valid_transactions.size() - new_block_transactions.size());
                for (const auto &tx : valid_transactions) {
                    bool was_included = false;
                    for (const auto &id : included_ids) if (tx.getTransactionId() == id) { was_included = true; break; }
                    if (!was_included) remaining2.push_back(tx);
                }
                valid_transactions = std::move(remaining2);
            } else {
                // Still nothing included -> no progress possible (insufficient
                // balances for all remaining txs). Stop to avoid infinite loop.
                cout << "No transactions could be included in this pass (insufficient balances). Stopping. Remaining: " << valid_transactions.size() << "\n";
                // leave mempool as-is for inspection, but clear to stop main loop
                valid_transactions.clear();
            }
        } else {
            // normal case: some txs included, keep the remaining for next blocks
            valid_transactions = std::move(remaining);
        }
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
    new_block.difficulty = new_block.difficulty + 0.1*new_block.height;

    block_hash data = get_block_hash_threaded(new_block);

    new_block.curr_block_hash = data.hash;
    new_block.nonce = data.nonce;
    new_block.core = data.core;

    // Print a concise block summary
    /*cout << "Block #" << new_block.height << " mined: nonce=" << new_block.nonce
        << " hash=" << new_block.curr_block_hash << " merkle=" << new_block.merkle_root_hash
        << " tx_count=" << new_block.transactions.size() << endl;
    */

    print_block(new_block);
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

    // To increase the chance that transactions are accepted when
    // blocks are assembled, generate smaller transactions on average.
    // Use a fraction of the sender's balance (here 1/4) as the
    // maximum amount for a single generated transaction.
    int sender_balance = users[sender_idx].getBalance();
    int max_amt = (sender_balance / 4) >= 1 ? (sender_balance / 4) : 1;
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

void print_block(const block& b) {
    cout << "\n"
         << "╔══════════════════════════════════════════════════════════╗\n"
         << "║                     🧱 BLOCK DETAILS                     ║\n"
         << "╚══════════════════════════════════════════════════════════╝\n\n";

    cout << left << setw(20) << "🔢 Height:"          << b.height << "\n"
         << left << setw(20) << "🧮 Mined on core:"          << b.core << "\n"
         << left << setw(19) << "⏰ Timestamp:"       << b.timestamp << "\n"
         << left << setw(23) << "⚙️  Version:"            << b.version << "\n"
         << left << setw(20) << "🎯 Difficulty:"       << b.difficulty << "\n"
         << left << setw(20) << "🔁 Nonce:"            << b.nonce << "\n\n"

         << left << setw(20) << "🔗 Prev Hash:"        << b.prev_block_hash << "\n"
         << left << setw(20) << "🔒 Curr Hash:"        << b.curr_block_hash << "\n"
         << left << setw(20) << "🌳 Merkle Root:"      << b.merkle_root_hash << "\n";

    cout << "\n📦 Transactions: " << setw(3) <<b.transactions.size() << "\n";
    cout << "───────────────────────────────────────────────────────────\n";

    if (b.transactions.empty()) {
        cout << "   (No transactions in this block)\n";
    }
}

void print_blockchain_summary(const vector<block>& blockchain) {
    cout << "\n"
         << "╔════════════════════════════════════════════════════════════════════════════════════════════════╗\n"
         << "║                                   📜 BLOCKCHAIN SUMMARY                                       ║\n"
         << "╚════════════════════════════════════════════════════════════════════════════════════════════════╝\n";

    if (blockchain.empty()) {
        cout << "\n🚫 The blockchain is empty. No blocks to display.\n\n";
        return;
    }

    cout << left
         << setw(8)  << "HEIGHT"
         << setw(25) << "TIMESTAMP"
         << setw(16) << "HASH"
         << setw(10) << "TX_COUNT"
         << setw(10) << "NONCE"
         << setw(12) << "DIFFICULTY"
         << setw(10) << "CORE"
         << endl;

    cout << string(95, '-') << "\n";

    for (const auto& b : blockchain) {
        cout << left
             << setw(8)  << b.height
             << setw(25) << b.timestamp.substr(0, 24)   // shorten timestamp if too long
             << setw(16) << (b.curr_block_hash.size() > 12 ? b.curr_block_hash.substr(0, 12) + "..." : b.curr_block_hash)
             << setw(10) << b.transactions.size()
             << setw(10) << b.nonce
             << setw(12) << b.difficulty
             << setw(10) << b.core
             << endl;
    }

    cout << string(95, '-') << "\n";
    cout << "Total blocks: " << blockchain.size() << "\n\n";
}

void print_blockchain_overview(const vector<block>& blockchain) {
    cout << "\n"
         << "╔══════════════════════════════════════════════════════════════╗\n"
         << "║                 🌍 BLOCKCHAIN OVERVIEW SUMMARY               ║\n"
         << "╚══════════════════════════════════════════════════════════════╝\n\n";

    if (blockchain.empty()) {
        cout << "🚫 The blockchain is empty.\n\n";
        return;
    }

    int total_blocks = blockchain.size();
    int total_transactions = 0;
    double total_difficulty = 0.0;
    int total_nonce = 0;

    for (const auto& b : blockchain) {
        total_transactions += static_cast<int>(b.transactions.size());
        total_difficulty += b.difficulty;
        total_nonce += b.nonce;
    }

    double avg_tx_per_block = total_transactions / static_cast<double>(total_blocks);
    double avg_difficulty = total_difficulty / total_blocks;
    double avg_nonce = total_nonce / static_cast<double>(total_blocks);

    const block& last_block = blockchain.back();

    cout << fixed << setprecision(2);

    cout << left << setw(25) << "📏 Total Blocks:"        << total_blocks << "\n"
         << left << setw(25) << "💸 Total Transactions:"  << total_transactions << "\n"
         << left << setw(25) << "📊 Avg TX per Block:"    << avg_tx_per_block << "\n"
         << left << setw(25) << "🎯 Avg Difficulty:"      << avg_difficulty << "\n"
         << left << setw(25) << "🔁 Avg Nonce:"           << avg_nonce << "\n"
         << left << setw(25) << "🔒 Latest Block Hash:"   << last_block.curr_block_hash << "\n"
         << left << setw(25) << "🧮 Latest Block Height:" << last_block.height << "\n"
         << left << setw(24) << "⏰ Last Timestamp:"      << last_block.timestamp << "\n";

    cout << "\n✅ Blockchain summary generated successfully.\n\n";
}