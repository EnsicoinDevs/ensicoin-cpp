#include "messages.hpp"

#include <rapidjson/document.h>
#include <string>

NotFound::NotFound(std::string resType, std::string resHash) : Message("notfound"), type(resType), hash(resHash) {}

NotFound::NotFound(rapidjson::Document* doc) : Message(doc), type((*doc)["message"]["type"].GetString()),
							     hash((*doc)["message"]["hash"].GetString()) {}

rapidjson::Value NotFound::json(rapidjson::Document* document) const{
	rapidjson::Value messageValue = Message::json(document);
	rapidjson::Value content(rapidjson::kObjectType);
	rapidjson::Value typeVal;
	rapidjson::Value hashVal;

	typeVal.SetString(type.c_str(), type.length(), document->GetAllocator());
	hashVal.SetString(hash.c_str(), hash.length(), document->GetAllocator());

	content.AddMember("type", typeVal, document->GetAllocator());
	content.AddMember("hash", hashVal, document->GetAllocator());

	messageValue.AddMember("message", content, document->GetAllocator());

	return messageValue;
}
