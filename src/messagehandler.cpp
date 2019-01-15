#include "messagehandler.hpp"
#include "messages.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"
#include "connection.hpp"

#include <algorithm>
#include <iostream>

namespace network{
	MessageHandler::MessageHandler(
			message::message_type tp,
			NetworkBuffer* buffer_,
			Node* node_,
			Connection::pointer conn_,
			std::shared_ptr<Logger> logger_) :
		buffer(buffer_),
		node(node_),
		conn(conn_),
		logger(logger_) {
		logger->trace("Handling message from ", conn->remote());
		switch(tp){
			case message_type::whoami:
				onWhoAmI();
				break;
			case message_type::whoamiack:
				onWhoAmIAck();
				break;
			case message_type::inv:
				onInv();
				break;
			default:
				onUnknown(tp);
				break;
		}
	}

	void MessageHandler::onUnknown(message::message_type tp){
		logger->warn("unknown message type : ", tp);
	}

	void MessageHandler::onInv(){
		std::vector<networkable::Inv_vect> unknown;
		auto msg = std::make_shared<message::Inv>(buffer);
		for(auto& res : msg->getData()){
			if( res.type == 
					networkable::Inv_vect::ressource_type::txRes){
				if(!node->transactionExists(res.hash))
					unknown.push_back(res);
			}
			else if( res.type ==
					networkable::Inv_vect::ressource_type::blockRes){
				if(!node->blockExists(res.hash)){
					unknown.push_back(res);
				}
			}
		}
		auto resp = std::make_shared<message::GetData>(unknown);
		conn->sendMessage(resp);
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
