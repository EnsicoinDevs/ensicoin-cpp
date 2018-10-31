#include <asio.hpp>
#include <memory>
#include <functional>
#include <stdexcept>

#include "node.hpp"
#include "messages.hpp"
#include "connection.hpp"

#include <iostream>

Node::Node(asio::io_context& io_context) : acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 4224)) {
	acceptor.listen();
	run();

	const std::string johynIP("78.248.188.120");
	const std::string myIP("82.235.104.10");
	auto messageTest = std::make_shared<WhoAmI>();
	
	Connection::pointer testConnection = Connection::create(io_context, this);
	testConnection->bind( asio::ip::address::from_string(myIP));

	testConnection->sendMessage(messageTest);
}

void Node::run(){
	Connection::pointer newConnection = Connection::create(acceptor.get_executor().context(), this);
	acceptor.async_accept(newConnection->getSocket(), std::bind( &Node::handleAccept, this, newConnection ));

	/*for(auto& conn : connections){
		conn->idle();
	}*/
}

void Node::handleAccept(Connection::pointer newConnection){
	newConnection->start();
	connections.push_back(newConnection);
	run();
}
