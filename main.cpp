#include <bits/stdc++.h>
#include "public_ledger.h"
#include "mempad.h"
#include "wallet.h"
#include "Transaction.h"
#include "network.h"

using namespace std;

void delete_transactions_file() {
    if (remove("transactions.json") == 0) {
        cout << "\n[🗑️] Deleted transactions.json after broadcasting!\n";
    } else {
        cout << "\n[⚠️] Warning: transactions.json could not be deleted!\n";
    }
}

int main() {
    io_service io;

    // Step 1: Create Wallet
    Wallet w1;  

    // Step 2: Create Transactions
    vector<Transaction> transactions;
    for (int i = 0; i < 3; i++) {
        string receiver;
        int amount;
        cout << "Enter Receiver Public Key: ";
        cin >> receiver;
        cout << "Enter Amount: ";
        cin >> amount;
        transactions.emplace_back(w1.public_key, receiver, amount);
        transactions.back().save_to_file();
    }

    // Step 3: Broadcast Transactions
    broadcast_transaction(io);
    delete_transactions_file();

    // Step 4: Receive Transactions
    receive_transaction(io);

    // Step 5: Process Transactions in Mempool
    mempool m1;
    vector<string> all_trans_in_string_format = m1.convert_json_to_strings();

    // Step 6: Start Mining
    Blockchain miniBTC_chain;
    miniBTC_chain.add_block(all_trans_in_string_format);

    // Step 7: Display Updated Blockchain
    miniBTC_chain.display_chain();

    return 0;
}
