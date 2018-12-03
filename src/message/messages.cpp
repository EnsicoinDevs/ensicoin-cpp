#include "messages.hpp"
#include "constants.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <iostream>
#include <string>

namespace message{
	
	std::string Message::getTypeAsString() const{
		std::string tp;
		switch(type){
			case whoami:
				tp = "whoami";
				break;
			case whoamiack:
				tp = "whoamiack";
				break;
			case getaddr:
				tp = "getaddr";
				break;
			case addr:
				tp =  "addr";
				break;
			case inv:
				tp = "inv";
				break;
			case getdata:
				tp = "getdata";
				break;
			case notfound:
				tp = "notfound";
				break;
			case block:
				tp = "block";
				break;
			case tx:
				tp = "tx";
				break;
			case getblocks:
				tp = "getblocks";
				break;
			case getmempool:
				tp = "getmempool";
				break;
			case unknown:
				tp = "unknown";
				break;
		}
		return tp;
	}

	Message::message_type Message::typeFromString(const std::string& typeStrPadded){
		std::istringstream is(typeStrPadded);
		std::string typeStr;
		is >> typeStr;
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
