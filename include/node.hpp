#ifndef NODE_HPP
#define NODE_HPP

#include "blocks.hpp"
#include "transaction.hpp"
#include "hashmemory.hpp"
#include "messages.hpp"

#include <map>
#include <asio.hpp>

class Node{
	private:
		int version;

//		HashMemory mainChain;
//		std::vector< HashMemory > secondaryChains;
//		std::vector<Block> orphanBlock;
		
//		HashMemory mempool;
		
		asio::io_context io_context;
		asio::ip::tcp::socket socket;
	public:
		Node();
		asio::error_code connect(std::string ipAdress);
		asio::error_code sendMessage(Message* message);
};

#endif /* NODE_HPP */
