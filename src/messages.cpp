#include <string>
#include <ctime>
#include <rapidjson/document.h>

#include "messages.hpp"

using namespace rapidjson;

Message::Message(std::string messageType) : magic(42), type(messageType), timestamp(std::time(0)){}

const std::string Message::getType() const{
	return type;
}

Value Message::json(Document* document) const{
	Value messageValue(kObjectType);
	messageValue.AddMember("magic",magic, document->GetAllocator());

	Value typeStr;
	typeStr.SetString(type.c_str(), type.length(), document->GetAllocator());
	messageValue.AddMember("type",typeStr, document->GetAllocator());

	messageValue.AddMember("timestamp", timestamp, document->GetAllocator());

	return messageValue;
}

WhoAmI::WhoAmI() : Message("whoami"), version(0) {}

Value WhoAmI::json(Document* document) const{
	Value messageValue = Message::json(document);
	Value messageContent(kObjectType);
	messageContent.AddMember("version",version,document->GetAllocator());
	messageValue.AddMember("message",messageContent,document->GetAllocator());

	return messageValue;
}
