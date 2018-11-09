#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include "messages.hpp"

#include <asio.hpp>
#include <memory>
#include <rapidjson/document.h>

class Node;

class Connection : public std::enable_shared_from_this<Connection>{
	public:
		using pointer = std::shared_ptr<Connection>;
		static pointer create(asio::io_context& io_context, Node* node);
		
		std::string remote() const;
		asio::ip::tcp::socket& getSocket();
		void start();
		void sendMessage(Message::messagePointer message);
		void bind(asio::ip::address address);
		void idle();

		void wave();
	private:
		Connection(asio::io_context& io_context, Node* node);

		void handleRead();
		void handleWrite(std::string type);
			
		void resetBuffer();

		asio::ip::tcp::socket socket;
		
		Node* node;
		
		bool waved;
		bool connected;
		
		std::vector<Message::messagePointer> bufferedMessages;
		asio::streambuf buffer;
};

#endif /* CONNECTION_HPP */
