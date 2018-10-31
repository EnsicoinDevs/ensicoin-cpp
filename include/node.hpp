#ifndef NODE_HPP
#define NODE_HPP

#include "blocks.hpp"
#include "transaction.hpp"
#include "hashmemory.hpp"
#include "connection.hpp"

#include <asio.hpp>
#include <vector>
#include <memory>

class Node{
	private:
		int version;

//		HashMemory mainChain;
//		std::vector< HashMemory > secondaryChains;
//		std::vector<Block> orphanBlock;
		
//		HashMemory mempool;
		
		asio::ip::tcp::acceptor acceptor;
		std::vector<Connection::pointer> connections;
		
		void handleAccept(Connection::pointer newConnection);
	public:
		Node(asio::io_context& io_context);
		void run();
};

#endif /* NODE_HPP */
