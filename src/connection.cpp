#include "connection.hpp"
#include "constants.hpp"
#include "messagehandler.hpp"
#include "messages.hpp"
#include "util.hpp"

#include <asio.hpp>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace network{

	Connection::pointer Connection::create(asio::io_context& io_context, Node* node){
		return Connection::pointer(new Connection(io_context, node));
	}

	asio::ip::tcp::socket& Connection::getSocket(){
		return socket;
	}

	std::string Connection::remote() const{
		return socket.remote_endpoint().address().to_string();
	}

	void Connection::bind(asio::ip::address ipAddress){
		socket.connect(asio::ip::tcp::endpoint( ipAddress, constants::PORT));
		sendMessage(std::make_shared<message::WhoAmI>());
		waved = true;
	}

	void Connection::start(){
		idle();
	}

	void Connection::sendMessage(message::Message::pointer message){
		if(message->getType() == message::Message::\
				message_type::whoami || connected){
			const std::string messageStr = message->byteRepr();
			asio::async_write(socket, asio::buffer(messageStr), 
					std::bind(&Connection::handleWrite, 
								shared_from_this(), 
								message->getTypeAsString()));
		}
		else
			bufferedMessages.push_back(message);
	}

	Connection::Connection(asio::io_context& io_context, Node* nodePtr) : 
		socket(io_context),
		node(nodePtr), 
		versionUsed(constants::VERSION),
		waved(false), 
		connected(false) {}

	void Connection::wave(int connectionVersion){
		if(!waved){
			auto response = std::make_shared<message::WhoAmI>();
			sendMessage(response);
			waved = true;
			if(versionUsed > connectionVersion){
				versionUsed = connectionVersion;
			}
		}
		if(waved){
			auto response = std::make_shared<message::WhoAmIAck>();
			sendMessage(response);
		}
	}

	void Connection::acknowledge(){
		if(!waved){
			std::cerr << "Connection to " << remote() <<
				" can't be validated if not waved" << std::endl;
		}
		else{
			connected = true;
		}
	};

	std::string Connection::readAll(){
		std::istream is(&buffer);
		std::string stringData(
				std::istreambuf_iterator<char>(is), {});
		return stringData;
	}

	void Connection::handleHeader(){
		std::cerr << "Reading header" << std::endl;
		auto stringData = readAll();
		netBuffer.appendRawData(stringData);
		auto header = networkable::MessageHeader(&netBuffer);
		asio::async_read(socket, buffer, 
				asio::transfer_exactly(header.payloadLength), 
				std::bind(&Connection::handleMessage, 
					shared_from_this(), header) );
	}

	void Connection::handleMessage(const networkable::MessageHeader& header){
		std::cout << header.type << " from " << remote() << std::endl;
		auto stringData = readAll();
		netBuffer.appendRawData(stringData);
		MessageHandler(message::Message::typeFromString(header.type),
					   &netBuffer,
					   node,
					   shared_from_this());
		idle();
	}

	void Connection::handleWrite(const std::string& type){
		std::cerr << type << " to " << remote() << std::endl;
		/*if(type == whoami)
			waved = true;*/
		idle();
	}

	void Connection::idle(){
		asio::async_read(socket, buffer, 
				asio::transfer_exactly(networkable::MessageHeader::size), 
				std::bind(&Connection::handleHeader, 
					shared_from_this()) );

		std::cout << "Waiting message" << std::endl;
		
		if( connected ){
			while(!bufferedMessages.empty()){
				sendMessage(bufferedMessages.back());
				bufferedMessages.pop_back();
			}
		}
	}
} //namespace network
