#include "messagehandler.hpp"
#include "messages.hpp"
#include "networkbuffer.hpp"
#include "connection.hpp"

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
		auto msg = std::make_shared<message::WhoAmI>(buffer);
		conn->wave(msg->getVersion());
	}

	void MessageHandler::onWhoAmIAck(){
		conn->acknowledge();
	}
} // namespace network
