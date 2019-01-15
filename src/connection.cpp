#include "connection.hpp"
#include "constants.hpp"
#include "messagehandler.hpp"
#include "messages.hpp"
#include "util.hpp"

#include <asio.hpp>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace network{

	Connection::pointer Connection::create(
			asio::io_context& io_context,
			Node* node,
			std::shared_ptr<Logger> lgr){
		return Connection::pointer(new Connection(io_context, node, lgr));
	}

	asio::ip::tcp::socket& Connection::getSocket(){
		return socket;
	}

	std::string Connection::remote() const{
		std::ostringstream os;
		os << socket.remote_endpoint().address().to_string() << ":" << socket.remote_endpoint().port();
		return os.str();
	}

	void Connection::bind(asio::ip::address ipAddress){
		socket.connect(asio::ip::tcp::endpoint( ipAddress,
					constants::PORT));
		currentStatus = Initiated;

		sendMessage(std::make_shared<message::WhoAmI>(
					networkable::Address(time(nullptr), 
						networkable::IP(constants::IP_bytes),
						socket.local_endpoint().port()))
				);
	}

	void Connection::start(){
		idle();
	}

	void Connection::sendMessage(message::Message::pointer message){
		if(message->getType() == message::\
				message_type::whoami || 
				message->getType() == message::\
				message_type::whoamiack ||
				currentStatus == Ack){
			const std::string messageStr = message->byteRepr();
			//util::printBinaryString(messageStr);
			asio::async_write(socket, asio::buffer(messageStr), 
					std::bind(&Connection::handleWrite, 
						shared_from_this(), 
						message::getTypeAsString(message->getType())));
		}
		else
			bufferedMessages.push_back(message);
	}

	Connection::Connection(asio::io_context& io_context,
			Node* nodePtr,
			std::shared_ptr<Logger> logger_) :
		socket(io_context),
		node(nodePtr), 
		versionUsed(constants::VERSION),
		currentStatus(Idle),
		netBuffer(logger_),
		logger(logger_),
		peerType(peer) {}

	void Connection::updateStatus(int connectionVersion,
			type peerType_){
		if( peerType == peer){
			peerType = peerType_;
		}
		switch(currentStatus){
			case Idle:{
						  if(connectionVersion < versionUsed)
							  versionUsed = connectionVersion;
						  auto v4Bytes = socket.local_endpoint().address().to_v4().to_bytes();
						  std::array<unsigned char, 16> bytes;
						  bytes.fill((unsigned char)(0xff));
						  for(int i = 0; i < 4; ++i){
							  bytes[16-4+i] = v4Bytes[i];
						  }

						  sendMessage(std::make_shared<message::WhoAmI>(
									  networkable::Address(time(nullptr), 
										  networkable::IP(bytes),
										  socket.local_endpoint().port()))
								  );
						  sendMessage(std::make_shared<message::WhoAmIAck>());
						  currentStatus = WaitingAck;
						  logger->trace("[",remote(),"] Idle->WaitingAck");
						  break;
					  }
			case WaitingAck:{
								currentStatus = Ack;
								node->registerConnection(shared_from_this());
								if(connectionVersion < versionUsed)
									versionUsed = connectionVersion;
								logger->trace("[",remote(),"] WaitingAck->Ack");
								break;
							}

			case Initiated:{
							   currentStatus = Waiting;
							   logger->trace("[",remote() ,"] Initiated->Waiting");
							   break;
						   }
			case Waiting:{
							 node->registerConnection(shared_from_this());
							 if(connectionVersion < versionUsed)
								 versionUsed = connectionVersion;
							 currentStatus = Ack;
							 sendMessage(std::make_shared<message::WhoAmIAck>());
							 logger->trace("[", remote(), "] Waiting->Ack");
							 break;
						 }

			case Ack:{
						 logger->warn("[",remote(),"] is already Ack");
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
		logger->trace("[", remote(), "] reading header");
		auto stringData = readAll();
		NetworkBuffer headerBuffer(stringData, logger);
		auto header = networkable::MessageHeader(&headerBuffer);
		if(header.payloadLength > 0) {
			asio::async_read(socket, buffer, 
				asio::transfer_exactly(header.payloadLength), 
				std::bind(&Connection::handleMessage, 
					shared_from_this(), header) );
		} else {
			handleMessage(header);
		}
	}

	void Connection::handleMessage(const networkable::MessageHeader& header){
		logger->info(message::getTypeFromString(header.type), " from ", remote());
		logger->trace("[", remote(),"] reading ", header.payloadLength, " bytes in payload");
		auto stringData = readAll();
		//std::cout << "Payload :" << std::endl;
		//util::printBinaryString(stringData);
		NetworkBuffer messageBuffer(stringData, logger);
		MessageHandler(message::getTypeFromString(header.type),
				&messageBuffer,
				node,
				shared_from_this(),
				logger);
		idle();
	}

	void Connection::handleWrite(const std::string& messageType){
		logger->info(messageType," to ", remote());
		idle();
	}

	void Connection::idle(){
		asio::async_read(socket, buffer, 
				asio::transfer_exactly(networkable::MessageHeader::size), 
				std::bind(&Connection::handleHeader, 
					shared_from_this()) );

		logger->trace("[", remote(), "] waiting message");

		if( currentStatus == Ack){
			while(!bufferedMessages.empty()){
				sendMessage(bufferedMessages.back());
				bufferedMessages.pop_back();
			}
		}
	}
} //namespace network
