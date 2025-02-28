#include "network.h"

vector<string> peer_ips = { "192.168.246.31", "192.168.246.29" }; // Your IPs

void delete_transactions_file() {
    if (remove("transactions.json") == 0) {
        cout << "\n[🗑️] Deleted transactions.json after broadcasting!\n";
    } else {
        cout << "\n[⚠️] Warning: transactions.json could not be deleted!\n";
    }
}

void receive_transaction(io_service& io) {
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 12345)); 
    tcp::socket socket(io);
    acceptor.accept(socket);

    cout << "[📥] Receiving transactions.json file..." << endl;
    ofstream outFile("transactions.json", ios::binary);
    if (!outFile) {
        cerr << "[❌] Error: Could not open transactions.json for writing!" << endl;
        return;
    }

    char buffer[1024];
    boost::system::error_code ec;
    while (true) {
        size_t len = socket.read_some(boost::asio::buffer(buffer), ec);
        if (ec == boost::asio::error::eof) break; 
        if (ec) {
            cerr << "[❌] Error: " << ec.message() << endl;
            break;
        }
        outFile.write(buffer, len);
    }
    outFile.close();
    cout << "[✅] Transaction received and saved!" << endl;
}

void broadcast_transaction(io_service& io) {
    bool broadcast_successful = false; // Track if at least one transaction was sent

    for (const string& ip : peer_ips) {
        tcp::socket socket(io);
        boost::system::error_code ec;
        socket.connect(tcp::endpoint(ip::address::from_string(ip), 12345), ec);

        if (ec) {
            cerr << "[❌] Connection to " << ip << " failed: " << ec.message() << endl;
            continue;  // Try the next IP
        }

        cout << "[📤] Sending transactions.json to " << ip << "...\n";

        ifstream inFile("transactions.json", ios::binary);
        if (!inFile) {
            cerr << "[❌] Error: Could not open transactions.json for reading!" << endl;
            return;
        }

        char buffer[1024];
        while (!inFile.eof()) {
            inFile.read(buffer, sizeof(buffer));
            size_t bytesRead = inFile.gcount();
            socket.write_some(boost::asio::buffer(buffer, bytesRead), ec);
            if (ec) {
                cerr << "[❌] Error sending data: " << ec.message() << endl;
                break;
            }
        }
        inFile.close();
        cout << "[✅] File sent to " << ip << "!\n";
        broadcast_successful = true; // At least one transaction sent successfully
    }

    if (broadcast_successful) {
        delete_transactions_file();  // ✅ Delete only if broadcast was successful
    } else {
        cout << "[⚠️] Warning: Transaction file not deleted as no successful broadcasts happened.\n";
    }
}
