#include <asio.hpp>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "node.hpp"
#include "messages.hpp"

#include <iostream>

Node::Node() : acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 4224)) {}

asio::ip::tcp::socket Node::sendMessage(Message* message, asio::ip::address address){
	asio::error_code error;
	
	rapidjson::Document* d = new Document();
	d->SetObject();
	
	rapidjson::Value messageJson = message->json(d);

	rapidjson::StringBuffer jsonBuffer;
	rapidjson::Writer<StringBuffer> writer(jsonBuffer);
	messageJson.Accept(writer);

	const std::string messageStr = jsonBuffer.GetString();
	
	asio::ip::tcp::socket socket(io_context);
	socket.connect(asio::ip::tcp::endpoint(address, 4224));
	asio::write(socket, asio::buffer(messageStr), error);

	return socket;
}

rapidjson::Document Node::readJSON(asio::ip::tcp::socket& socket){
	asio::streambuf recv_buf;
	asio::error_code error;
	rapidjson::Document d;
	
	int readChars(0);

	do{
	asio::read(socket, recv_buf, asio::transfer_exactly(1));
	readChars++;

	d.Parse(asio::buffer_cast<const char*>(recv_buf.data()));
	}while(d.HasParseError() || readChars <= MESSAGE_LIMIT);

	return d;
}
