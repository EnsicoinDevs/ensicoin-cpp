#include "messages.hpp"
#include "constants.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <iostream>
#include <string>

namespace message{
	
	std::string Message::getTypeAsString() const{
		switch(type){
			case whoami:
				return "whoami";
			case whoamiack:
				return "whoamiack";
			case getaddr:
				return "getaddr";
			case addr:
				return "addr";
			case inv:
				return "inv";
			case getdata:
				return "getdata";
			case notfound:
				return "notfound";
			case block:
				return "block";
			case tx:
				return "tx";
			case getblocks:
				return "getblocks";
			case getmempool:
				return "getmempool";
			case unknown:
				return "unknown";
		}
	}

	Message::message_type Message::typeFromString(const std::string& typeStr){
		if(typeStr == "whoami")
			return whoami;
		else if(typeStr == "whoamiack")
			return whoamiack;
		else if(typeStr == "getaddr")
			return getaddr;
		else if(typeStr == "addr")
			return addr;
		else if(typeStr == "inv")
			return inv;
		else if(typeStr == "getdata")
			return getdata;
		else if(typeStr == "notfound")
			return notfound;
		else if(typeStr == "block")
			return block;
		else if(typeStr == "tx")
			return tx;
		else if(typeStr == "getblocks")
			return getblocks;
		else if(typeStr == "getmempool")
			return getmempool;
		else
			return unknown;
	}

	Message::Message(message_type messageType) : magic(constants::MAGIC), 
		type(messageType) {}

	Message::message_type Message::getType() const{
		return type;
	}

	std::string Message::byteRepr() const{
		auto strType = getTypeAsString();
		strType += std::string(12-strType.size(), char(0x00));
		auto payloadString = this->payload();
		auto payloadLength = payloadString.size()/2;
		networkable::MessageHeader header(magic,strType,payloadLength);
		return 	header.byteRepr() +
			payloadString;
	}

} // namespace message
