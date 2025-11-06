#include "header.h"

const int num_threads = 3;

const double diff = 5;

int main() {


    // Generuoja userius 
    vector<user> users = generate_users(1000);
    
    // Generuoja transakcijas 
    vector<transaction> transactions = generate_transactions(10000, users);

    // Validuoja transakcijas
    vector<transaction> valid_transactions = validate_transactions(users, transactions);

    // Show how many transactions passed pre-inclusion validation
    cout << "After validation: " << valid_transactions.size() << " transactions kept (out of " << transactions.size() << ")" << endl;

    for (size_t i = 0; i < valid_transactions.size(); i++) {

        //cout <<"["<<i<<"] " << transactions[i].sender_hash << " " << transactions[i].receiver_hash << " " << transactions[i].amount << endl;
        //cout <<"["<<i<<"] " << valid_transactions[i].hash << " " << valid_transactions[i].amount << endl;
    }

    // Sukuria blockchaina
    vector<block> blockchain;

    // Manually sukuriamas pirmas blokas
    block genesis = build_genesis_block();

    //cout << genesis.nonce << " "<< genesis.curr_block_hash<< " real" << endl;

    // Genesis blokas pridedamas i blockchaina
    blockchain.push_back(genesis);

    // Blokai pridedami i blockchaina iki kol nielieka transakciju
    while (valid_transactions.size() > 0) {
        add_block(blockchain,valid_transactions, users);
        //cout << "Blokchain size: " <<blockchain.size()<< " " << "Transactions left: "<< valid_transactions.size()<<endl << endl;
    }

    print_blockchain_summary(blockchain);
   
    print_blockchain_overview(blockchain);
}