#include "header.h"
#include <cstdlib>
#include <string>
#include <vector>

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

int main() {

    vector<user> users;

    for (int i = 0; i < 1000; i++){
        user u;
        u.name = get_name();
        u.balance = get_balance();
        u.hash = hashing(u.name + to_string(u.balance));

        users.push_back(u);

        //cout <<users[i].name<< " "<< users[i].balance << " " << users[i].hash << endl;
    }

    vector<transaction> transactions;

    for (int i = 0; i < 10000; i++) {
        transaction t;
        t.sender_hash = users[random_int(0, users.size()-1)].hash;
        t.receiver_hash = users[random_int(0, users.size()-1)].hash;
        t.amount = get_balance();
        t.hash = hashing(t.sender_hash+t.receiver_hash+to_string(t.amount));
        transactions.push_back(t);

        //cout <<"["<<i<<"] " << transactions[i].sender_hash << " " << transactions[i].receiver_hash << " " << transactions[i].amount << endl;
        cout <<"["<<i<<"] " << transactions[i].hash << " " << transactions[i].amount << endl;
    }

    check_transactions()

}