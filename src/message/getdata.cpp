#include "messages.hpp"

#include <memory>
#include <rapidjson/document.h>
#include <string>
#include <vector>
#include <iostream>

std::string GetData::dataType() const{
	return invData.type;
}

std::vector<std::string> GetData::dataAsked() const{
	return invData.hashes;
}

GetData::GetData(InvData inv) : Message("getdata"), invData(inv) {}

GetData::GetData(rapidjson::Document* doc) : Message(doc), invData(InvData(&(*doc)["message"]["inv"])) {}

rapidjson::Value GetData::json(rapidjson::Document* document) const{
	rapidjson::Value messageValue = Message::json(document);
	rapidjson::Value invValue = invData.json(document);
	rapidjson::Value content(rapidjson::kObjectType);
	content.AddMember("inv", invValue, document->GetAllocator());
	
	messageValue.AddMember("message", content, document->GetAllocator());

	return messageValue;
}
