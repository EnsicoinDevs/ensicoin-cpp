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
			Connection::pointer conn_) :
		buffer(buffer_),
		node(node_),
		conn(conn_) {
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
		std::cerr << "Unknown message type" << std::endl;
	}

	void MessageHandler::onWhoAmI(){
		auto msg = std::make_shared<message::WhoAmI>(buffer);
		conn->wave(msg->getVersion());
	}

	void MessageHandler::onWhoAmIAck(){
		conn->acknowledge();
	}
} // namespace network
