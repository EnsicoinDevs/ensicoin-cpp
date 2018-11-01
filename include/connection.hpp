#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "messages.hpp"

#include <asio.hpp>
#include <memory>
#include <rapidjson/document.h>


const int MESSAGE_LIMIT = 3 * 1024;

class Node;

class Connection : public std::enable_shared_from_this<Connection>{
	public:
		using pointer = std::shared_ptr<Connection>;
		static pointer create(asio::io_context& io_context, Node* node);
		
		asio::ip::tcp::socket& getSocket();
		void start();
		void sendMessage(Message::messagePointer message);
		void bind(asio::ip::address address);
		void idle();
	private:
		Connection(asio::io_context& io_context, Node* node);
		void handleMessage(Message::messagePointer message);

		void handleRead();
		void handleWrite(std::string type);
			
		void resetBuffer();

		bool waved;
		asio::streambuf buffer;
		Node* node;
		asio::ip::tcp::socket socket;
};

#endif /* CONNECTION_HPP */
