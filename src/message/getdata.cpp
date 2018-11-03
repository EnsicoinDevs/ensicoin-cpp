#include "messages.hpp"

#include <memory>
#include <rapidjson/document.h>
#include <iostream>

GetData::GetData(InvData inv) : Message("getdata"), invData(inv) {}

GetData::GetData(rapidjson::Document* doc) : Message(doc), invData(InvData(&(*doc)["message"]["inv"])) {}

rapidjson::Value GetData::json(rapidjson::Document* document) const{
	rapidjson::Value messageValue = Message::json(document);
	rapidjson::Value invValue = invData.json(document);
	rapidjson::Value content(rapidjson::kObjectType);
	content.AddMember("inv", invValue, document->GetAllocator());
	
	messageValue.AddMember("messsage", content, document->GetAllocator());

	return messageValue;
}
