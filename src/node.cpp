#include <asio.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "node.hpp"
#include "messages.hpp"

#include <iostream>

Node::Node() : socket(io_context) {}

asio::error_code Node::connect(std::string ipAddress){
	asio::error_code error;
	socket.connect( asio::ip::tcp::endpoint(asio::ip::address::from_string(ipAddress), 4224), error);
	return error;
}

asio::error_code Node::sendMessage(Message* message){
	asio::error_code error;
	
	rapidjson::Document* d = new Document();
	d->SetObject();
	
	rapidjson::Value messageJson = message->json(d);

	rapidjson::StringBuffer jsonBuffer;
	rapidjson::Writer<StringBuffer> writer(jsonBuffer);
	messageJson.Accept(writer);

	const std::string messageStr = jsonBuffer.GetString();
	asio::write(socket, asio::buffer(messageStr), error);

	return error;
}
