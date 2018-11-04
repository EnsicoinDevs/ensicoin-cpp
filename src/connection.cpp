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
}

void Connection::start(){
	resetBuffer();
	idle();
}

void Connection::sendMessage(Message::messagePointer message){
	if(message->getType() == "whoami" || connected){
		const std::string messageStr = message->str();
		asio::async_write(socket, asio::buffer(messageStr), std::bind(&Connection::handleWrite, shared_from_this(), message->getType()));
	}
	else
		bufferedMessages.push_back(message);
}

Connection::Connection(asio::io_context& io_context, Node* nodePtr) : socket(io_context), node(nodePtr), waved(false), connected(false) {}

void Connection::handleRead(){
	rapidjson::Document doc;
	doc.SetObject();
	
	std::string strData = asio::buffer_cast<const char*>(buffer.data());
	if(strData.length() > 0 && !(strData[0] == '{'))
		strData = "{" + strData;
	
	auto jsonData = strData.c_str();

	if( strlen(jsonData) > MESSAGE_LIMIT)
		throw std::runtime_error("Message too long/Invalid JSON");

	doc.Parse(jsonData, strlen(jsonData));
		
	// std::cerr << "Json : " << jsonData << std::endl;

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
		else if ( messageType == "getdata")
			message = std::make_shared<GetData>(&doc);
		else if ( messageType == "notfound")
			message = std::make_shared<NotFound>(&doc);
		else if ( messageType == "block")
			message = std::make_shared<BlockMessage>(&doc);
		else if ( messageType == "transaction")
			message = std::make_shared<TransactionMessage>(&doc);
		else if ( messageType == "getblocks")
			message = std::make_shared<GetBlocks>(&doc);
		else if ( messageType == "getmempool")
			message = std::make_shared<GetMempool>(&doc);
		else
			throw std::runtime_error("Unknow message type : " + messageType);
		handleMessage(message);
	}
}

void Connection::handleWrite(std::string type){
	std::cerr << type << " to " << socket.remote_endpoint().address().to_string() << std::endl;
	if(type == "whoami")
		waved = true;
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
		if(waved)
			connected = true;
	}
	else if (messageType == "inv")
		std::cout << message->str() << std::endl;
	else if (messageType == "getdata")
		std::cout << message->str() << std::endl;
	else if (messageType == "notfound")
		std::cout << message->str() << std::endl;
	else if (messageType == "block")
		std::cout << message->str() << std::endl;
	else if (messageType == "transaction")
		std::cout << message->str() << std::endl;
	else if (messageType == "getblocks")
		std::cout << message->str() << std::endl;
	else if (messageType == "getmempool")
		std::cout << message->str() << std::endl;
	idle();
}

void Connection::idle(){
	asio::async_read(socket, buffer, asio::transfer_exactly(1), std::bind(&Connection::handleRead, shared_from_this()) );
	if( connected ){
		while(!bufferedMessages.empty()){
			sendMessage(bufferedMessages.back());
			bufferedMessages.pop_back();
		}
	}
}

void Connection::resetBuffer(){
	buffer.consume(buffer.size());
}
