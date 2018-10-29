#ifndef NODE_HPP
#define NODE_HPP

#include "blocks.hpp"
#include "transaction.hpp"
#include "hashmemory.hpp"
#include "messages.hpp"

#include <asio.hpp>
#include <map>

const int MESSAGE_LIMIT = 3*1024;

class Node{
	private:
		int version;

//		HashMemory mainChain;
//		std::vector< HashMemory > secondaryChains;
//		std::vector<Block> orphanBlock;
		
//		HashMemory mempool;
		
		asio::io_context io_context;
		asio::ip::tcp::acceptor acceptor;
	public:
		Node();
		asio::ip::tcp::socket sendMessage(Message* message,asio::ip::address ipAdress);
		rapidjson::Document readJSON(asio::ip::tcp::socket& socket);
};

#endif /* NODE_HPP */
