#include "connection.hpp"
#include "messages.hpp"

#include <asio.hpp>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <rapidjson/document.h>
#include <stdexcept>

Connection::pointer Connection::create(asio::io_context& io_context, Node* node){
	return Connection::pointer(new Connection(io_context, node));
}

asio::ip::tcp::socket& Connection::getSocket(){
	return socket;
}

void Connection::bind(asio::ip::address ipAddress){
	socket.connect(asio::ip::tcp::endpoint( ipAddress, 4224));
	sendMessage(std::make_shared<WhoAmI>());
	waved = true;
	start();
}

void Connection::start(){
	resetBuffer();
	std::ostream os(&buffer);

	os << "{";
	idle();
}

void Connection::sendMessage(Message::messagePointer message){
	const std::string messageStr = message->str();
	
	asio::async_write(socket, asio::buffer(messageStr), std::bind(&Connection::handleWrite, shared_from_this(), message->getType()));

}

Connection::Connection(asio::io_context& io_context, Node* node) : socket(io_context), node(node), waved(false) {}

void Connection::handleRead(){
	rapidjson::Document doc;
	doc.SetObject();
	
	auto jsonData = asio::buffer_cast<const char*>(buffer.data());
	if( strlen(jsonData) > MESSAGE_LIMIT)
		throw std::runtime_error("Message too long/Invalid JSON");

	doc.Parse(jsonData, strlen(jsonData));

	if (doc.HasParseError())
		idle();
	else{

		Message::messagePointer message;
		std::string messageType = doc["type"].GetString();

		resetBuffer();
		
		if ( messageType == "whoami")
			message = std::make_shared<WhoAmI>(&doc);
		else if ( messageType == "inv")
			message = std::make_shared<Inv>(&doc);
		else
			throw std::runtime_error("Unknow message type : " + messageType);
		handleMessage(message);
	}
}

void Connection::handleWrite(std::string type){
	std::cerr << type << " to " << socket.remote_endpoint().address().to_string() << std::endl;
	idle();
}

void Connection::handleMessage(Message::messagePointer message){
	
	std::cout << message->getType() << " from " << socket.remote_endpoint().address().to_string() << std::endl;

	auto messageType = message->getType();
	if (messageType == "whoami"){
		if(!waved){
			auto response = std::make_shared<WhoAmI>();
			sendMessage(response);
			waved = true;
		}
	}
	else if (messageType == "inv")
		std::cout << message->str() << std::endl;
	idle();
}

void Connection::idle(){
	asio::async_read(socket, buffer, asio::transfer_exactly(1), std::bind(&Connection::handleRead, shared_from_this()) );
}

void Connection::resetBuffer(){
	buffer.consume(buffer.size());
}
