#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;
using ip::tcp;

void receive_transaction(io_service& io);
void broadcast_transaction(io_service& io);
void delete_transactions_file();  // ✅ Added to ensure it's recognized
