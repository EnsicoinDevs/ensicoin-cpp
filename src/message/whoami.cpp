#include "messages.hpp"

#include "constants.hpp"

#include <rapidjson/document.h>
#include <string>

using rapidjson::Value;
using rapidjson::Document;

WhoAmI::WhoAmI() : Message("whoami"), version(VERSION) {}

WhoAmI::WhoAmI(rapidjson::Document* document) : Message(document), version((*document)["message"]["version"].GetInt()) {}

Value WhoAmI::json(Document* document) const{
	Value messageValue = Message::json(document);
	Value messageContent(rapidjson::kObjectType);
	messageContent.AddMember("version",version,document->GetAllocator());
	messageValue.AddMember("message",messageContent,document->GetAllocator());

	return messageValue;
}
