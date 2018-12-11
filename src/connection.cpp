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
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace network{

	Connection::pointer Connection::create(
			asio::io_context& io_context,
			Node* node,
			std::shared_ptr<spdlog::logger> lgr){
		return Connection::pointer(new Connection(io_context, node, lgr));
	}

	asio::ip::tcp::socket& Connection::getSocket(){
		return socket;
	}

	std::string Connection::remote() const{
		return socket.remote_endpoint().address().to_string();
	}

	void Connection::bind(asio::ip::address ipAddress){
		socket.connect(asio::ip::tcp::endpoint( ipAddress,
					constants::PORT));
		updateStatus(versionUsed);
	}

	void Connection::start(){
		idle();
	}

	void Connection::sendMessage(message::Message::pointer message){
		if(message->getType() == message::Message::\
				message_type::whoami || 
		   message->getType() == message::Message::\
		   		message_type::whoamiack ||
			currentStatus == Ack){
			const std::string messageStr = message->byteRepr();
			asio::async_write(socket, asio::buffer(messageStr), 
					std::bind(&Connection::handleWrite, 
								shared_from_this(), 
								message->getTypeAsString()));
		}
		else
			bufferedMessages.push_back(message);
	}

	Connection::Connection(asio::io_context& io_context,
			    		   Node* nodePtr,
						   std::shared_ptr<spdlog::logger> logger_) :
		socket(io_context),
		node(nodePtr), 
		versionUsed(constants::VERSION),
		currentStatus(Idle),
		netBuffer(logger_),
		logger(logger_) {}

	void Connection::updateStatus(int connectionVersion){
		switch(currentStatus){
			case Idle:{
				if(connectionVersion < versionUsed)
					versionUsed = connectionVersion;
				sendMessage(std::make_shared<message::WhoAmI>());
				sendMessage(std::make_shared<message::WhoAmIAck>());
				currentStatus = WaitingAck;
				logger->trace("[{}] Idle->WaitingAck", remote());
				break;
			}
			case WaitingAck:{
				currentStatus = Ack;
				logger->trace("[{}] WaitingAck->Ack", remote());
				break;
			}

			case Initiated:{
				currentStatus = Waiting;
				if(connectionVersion < versionUsed)
					versionUsed = connectionVersion;
				logger->trace("[{}] Initiated->Waiting", remote());
				break;
			}
			case Waiting:{
				currentStatus = Ack;
				sendMessage(std::make_shared<message::WhoAmIAck>());
				logger->trace("[{}] Waiting->Ack", remote());
				break;
			}

			case Ack:{
				logger->warn("[{}] is already Ack", remote());
			}
		}
	}

	std::string Connection::readAll(){
		std::istream is(&buffer);
		std::string stringData(
				std::istreambuf_iterator<char>(is), {});
		return stringData;
	}

	void Connection::handleHeader(){
		logger->debug("[{}] reading header", remote());
		auto stringData = readAll();
		netBuffer.appendRawData(stringData);
		auto header = networkable::MessageHeader(&netBuffer);
		asio::async_read(socket, buffer, 
				asio::transfer_exactly(header.payloadLength), 
				std::bind(&Connection::handleMessage, 
					shared_from_this(), header) );
	}

	void Connection::handleMessage(const networkable::MessageHeader& header){
		logger->info("{} from {}", header.type, remote());
		logger->debug("[{}] reading {} bytes in payload", remote(), header.payloadLength);
		auto stringData = readAll();
		netBuffer.appendRawData(stringData);
		MessageHandler(message::Message::typeFromString(header.type),
					   &netBuffer,
					   node,
					   shared_from_this(),
					   logger);
		idle();
	}

	void Connection::handleWrite(const std::string& type){
		logger->info("{} to {}", type, remote());
		idle();
	}

	void Connection::idle(){
		asio::async_read(socket, buffer, 
				asio::transfer_exactly(networkable::MessageHeader::size), 
				std::bind(&Connection::handleHeader, 
					shared_from_this()) );

		logger->trace("[{}] waiting mesage", remote());

		if( currentStatus == Ack){
			while(!bufferedMessages.empty()){
				sendMessage(bufferedMessages.back());
				bufferedMessages.pop_back();
			}
		}
	}
} //namespace network
