#include "messagehandler.hpp"
#include "messages.hpp"
#include "networkbuffer.hpp"
#include "connection.hpp"

#include <algorithm>
#include <iostream>

namespace network{
	MessageHandler::MessageHandler(
			message::Message::message_type tp,
			NetworkBuffer* buffer_,
			Node* node_,
			Connection::pointer conn_,
			std::shared_ptr<spdlog::logger> logger_) :
		buffer(buffer_),
		node(node_),
		conn(conn_),
		logger(logger_) {
		switch(tp){
			case message_type::whoami:
				onWhoAmI();
				break;
			case message_type::whoamiack:
				onWhoAmIAck();
				break;
			default:
				onUnknown();
				break;
		}
	}

	void MessageHandler::onUnknown(){
		logger->warn("unknown message type");
	}

	void MessageHandler::onWhoAmI(){
		auto tp = network::Connection::type::peer;
		auto msg = std::make_shared<message::WhoAmI>(buffer);
		const auto& services = msg->getServices();
		if(std::find(services.cbegin(), services.cend(), "wallet") !=
		   services.cend()){
			tp = network::Connection::type::walletpeer;
		}
		if(std::find(services.cbegin(), services.cend(), "relay") !=
		   services.cend()){
			tp = network::Connection::type::relaypeer;
		}
		if(std::find(services.cbegin(), services.cend(), "node") !=
		   services.cend()){
			tp = network::Connection::type::nodepeer;
		}
		conn->updateStatus(msg->getVersion(), tp);
	}

	void MessageHandler::onWhoAmIAck(){
		conn->updateStatus(0, network::Connection::type::peer);
	}
} // namespace network
