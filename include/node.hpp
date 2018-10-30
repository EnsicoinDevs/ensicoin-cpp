#ifndef NODE_HPP
#define NODE_HPP

#include "blocks.hpp"
#include "transaction.hpp"
#include "hashmemory.hpp"
#include "messages.hpp"

#include <asio.hpp>
#include <map>
#include <memory>

const int MESSAGE_LIMIT = 3*1024;

using socketPointer = std::shared_ptr<asio::ip::tcp::socket>;
using messagePointer = std::shared_ptr<Message>;

class Node{
	private:
		int version;

//		HashMemory mainChain;
//		std::vector< HashMemory > secondaryChains;
//		std::vector<Block> orphanBlock;
		
//		HashMemory mempool;
		
		asio::ip::tcp::acceptor acceptor;
		
		void sendMessage(messagePointer message, socketPointer socket);
		void readJSON(socketPointer socket, rapidjson::Document* document);
		std::shared_ptr<Message> readMessage(socketPointer socket);
		void handleConnection(socketPointer socket);
	public:
		Node(asio::io_context& io_context);
		void run();
};

#endif /* NODE_HPP */
