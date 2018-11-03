#include "messages.hpp"

#include <rapidjson/document.h>

GetMempool::GetMempool() : Message("getmempool") {}

GetMempool::GetMempool(rapidjson::Document* doc) : Message(doc) {}

rapidjson::Value GetMempool::json(rapidjson::Document* doc) const{
	auto messageValue = Message::json(doc);
	rapidjson::Value content(rapidjson::kObjectType);
	messageValue.AddMember("message", content, doc->GetAllocator());

	return messageValue;
}
