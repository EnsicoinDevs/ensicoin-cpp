#include "messages.hpp"

#include <rapidjson/document.h>
#include <string>

NotFound::NotFound(std::string type, std::string resHash) : Message("notfound"), resType(type), hash(resHash) {}

NotFound::NotFound(rapidjson::Document* doc) : Message(doc), resType((*doc)["message"]["type"].GetString()),
							     hash((*doc)["message"]["hash"].GetString()) {}

rapidjson::Value NotFound::json(rapidjson::Document* document) const{
	rapidjson::Value messageValue = Message::json(document);
	rapidjson::Value content(rapidjson::kObjectType);
	rapidjson::Value typeVal;
	rapidjson::Value hashVal;

	typeVal.SetString(resType.c_str(), resType.length(), document->GetAllocator());
	hashVal.SetString(hash.c_str(), hash.length(), document->GetAllocator());

	content.AddMember("type", typeVal, document->GetAllocator());
	content.AddMember("hash", hashVal, document->GetAllocator());

	messageValue.AddMember("message", content, document->GetAllocator());

	return messageValue;
}
