#ifndef NODE_HPP
#define NODE_HPP

#include "blockchain.hpp"
#include "blocks.hpp"
#include "connection.hpp"
#include "hashmemory.hpp"
#include "mempool.hpp"
#include "transaction.hpp"
#include "utxo.hpp"

#include <asio.hpp>
#include <memory>
#include <vector>

class Node{
	private:
//		HashMemory mainChain;
//		std::vector< HashMemory > secondaryChains;

		Mempool mempool;
		Blockchain blockchain;

		asio::ip::tcp::acceptor acceptor;
		std::vector<Connection::pointer> connections;
		
		void handleAccept(Connection::pointer newConnection);
	public:
		explicit Node(asio::io_context& io_context);
		void run();
};

#endif /* NODE_HPP */
