#include <asio.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <memory>
#include <functional>
#include <stdexcept>

#include "node.hpp"
#include "messages.hpp"

#include <iostream>

Node::Node(asio::io_context& io_context) : acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 4224)) {
	acceptor.listen();
	run();
}

void Node::run(){
	const std::string johynStringIP("78.248.188.120");
	auto messageTest = std::make_shared<WhoAmI>();
	
	socketPointer socketJohyn(new asio::ip::tcp::socket(acceptor.get_executor().context()));
	socketJohyn->connect(asio::ip::tcp::endpoint( asio::ip::address::from_string(johynStringIP), 4224));

	sendMessage(messageTest, socketJohyn);
	auto messageRecieved = readMessage(socketJohyn);

	std::cout << messageRecieved->str() << std::endl;

	for (;;){
		socketPointer pSocket = std::make_shared<asio::ip::tcp::socket>(acceptor.get_executor().context());
		acceptor.async_accept(*pSocket, std::bind( &Node::handleConnection, this, pSocket ));
	}
}

void Node::handleConnection(socketPointer socket){
	auto message = readMessage(socket);
	auto messageType = message->getType();
	if (messageType == "whoami"){
		auto response = std::make_shared<WhoAmI>();
		sendMessage(response, socket);
	}
}

void Node::sendMessage(messagePointer message, socketPointer socket){
	asio::error_code error;
	
	rapidjson::Document* d = new Document();
	d->SetObject();
	
	rapidjson::Value messageJson = message->json(d);

	rapidjson::StringBuffer jsonBuffer;
	rapidjson::Writer<StringBuffer> writer(jsonBuffer);
	messageJson.Accept(writer);

	const std::string messageStr = jsonBuffer.GetString();
	
	asio::write(*socket, asio::buffer(messageStr), error);

	std::cerr << message->getType() << " to " << socket->remote_endpoint().address().to_string() << std::endl;
}

void Node::readJSON(socketPointer socket, rapidjson::Document* d){
	asio::streambuf recv_buf;
	asio::error_code error;
	
	int readChars(0);

	do{
	asio::read(*socket, recv_buf, asio::transfer_exactly(1));
	readChars++;

	d->Parse(asio::buffer_cast<const char*>(recv_buf.data()));
	}while(d->HasParseError() && readChars <= MESSAGE_LIMIT);

	if (readChars > MESSAGE_LIMIT)
		throw std::runtime_error("Message Too long/Not JSON message");

	std::cerr << (*d)["type"].GetString() << " from " << socket->remote_endpoint().address().to_string() << std::endl;
}

std::shared_ptr<Message> Node::readMessage(socketPointer socket){
	rapidjson::Document* d = new rapidjson::Document();
	d->SetObject();

	readJSON(socket,d);

	std::shared_ptr<Message> message;
	std::string messageType = (*d)["type"].GetString();
	if ( messageType == "whoami")
		message = std::make_shared<WhoAmI>(d);
	else
		throw std::runtime_error("Unknow message type : " + messageType);

	return message;
};
