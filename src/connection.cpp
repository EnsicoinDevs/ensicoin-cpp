#include "connection.hpp"
#include "constants.hpp"
#include "messagehandler.hpp"
#include "messages.hpp"

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
		socket.connect(asio::ip::tcp::endpoint( ipAddress, PORT));
		sendMessage(std::make_shared<message::WhoAmI>());
	}

	void Connection::start(){
		resetBuffer();
		idle();
	}

	void Connection::sendMessage(message::Message::pointer message){
		if(message->getType() == message::Message::message_type::whoami || connected){
			const std::string messageStr = message->byteRepr();
			asio::async_write(socket, asio::buffer(messageStr), 
					std::bind(&Connection::handleWrite, 
								shared_from_this(), 
								message->getTypeAsString()));
		}
		else
			bufferedMessages.push_back(message);
	}

	Connection::Connection(asio::io_context& io_context, Node* nodePtr) : socket(io_context), node(nodePtr), waved(false), connected(false) {}

	void Connection::wave(){
		if(!waved){
			auto response = std::make_shared<message::WhoAmI>();
			sendMessage(response);
			waved = true;
		}
		if(waved)
			connected = true;
	}

	void Connection::handleRead(){
		std::string strData = asio::buffer_cast<const char*>(buffer.data());
		if(strData.length() > 0 && !(strData[0] == '{'))
			strData = "{" + strData;

		if( strData.size() > MESSAGE_LIMIT)
			throw std::runtime_error("Message too long");

		reader(strData, node, shared_from_this());
		idle();	
	}

	void Connection::handleWrite(std::string type){
		std::cerr << type << " to " << remote() << std::endl;
		/*if(type == whoami)
			waved = true;*/
		idle();
	}

	void Connection::idle(){
		asio::async_read(socket, buffer, 
				asio::transfer_exactly(reader.readRequest()), 
				std::bind(&Connection::handleRead, 
					shared_from_this()) );
		
		if( connected ){
			while(!bufferedMessages.empty()){
				sendMessage(bufferedMessages.back());
				bufferedMessages.pop_back();
			}
		}
	}

	void Connection::resetBuffer(){
		buffer.consume(reader.readCount());
	}

} //namespace network
