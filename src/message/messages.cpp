#include "messages.hpp"
#include "constants.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <iostream>
#include <string>

namespace message{
	
	Message::Message(message_type messageType) : magic(constants::MAGIC), 
		type(messageType) {}

	message_type Message::getType() const{
		return type;
	}

	std::string Message::byteRepr() const{
		auto strType = getTypeAsString(type);
		strType += std::string(12-strType.size(), char(0x00));
		auto payloadString = this->payload();
		auto payloadLength = payloadString.size();
		networkable::MessageHeader header(magic,strType,payloadLength);
		return 	header.byteRepr() +
				payloadString;
	}

} // namespace message
