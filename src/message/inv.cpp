#include "messages.hpp"

#include <memory>
#include <rapidjson/document.h>
#include <string>
#include <vector>
#include <iostream>

std::string Inv::getRessourceType() const{
	return data.type;
}

std::vector<std::string> Inv::getRessources() const{
	return data.hashes;
}

std::shared_ptr<GetData> Inv::respondRequest() const{
	return std::make_shared<GetData>(data);
}

InvData::InvData(rapidjson::Value* val) : type((*val)["type"].GetString()){
	auto& hashArray = (*val)["hashes"];
	for(auto& hash : hashArray.GetArray()){
		hashes.push_back(hash.GetString());
	}
}

InvData::InvData(std::string t, std::vector<std::string> hL) : type(t), hashes(hL) {}

rapidjson::Value InvData::json(rapidjson::Document* document) const {
	rapidjson::Value content(rapidjson::kObjectType);

	rapidjson::Value hashArray(rapidjson::kArrayType);
	for(auto& resHash : hashes){
		rapidjson::Value strVal;
		strVal.SetString(resHash.c_str(), resHash.length(), document->GetAllocator());
		hashArray.PushBack(strVal, document->GetAllocator());
	}
	rapidjson::Value typeVal;
	typeVal.SetString(type.c_str(), type.length(), document->GetAllocator());

	content.AddMember("type", typeVal, document->GetAllocator());
	content.AddMember("hashes", hashArray, document->GetAllocator());
	return content;
}

Inv::Inv(InvData dt) : Message("inv"), data(dt)  {}

Inv::Inv(rapidjson::Document* doc) : Message(doc), data(InvData(doc))  {
}

rapidjson::Value Inv::json(rapidjson::Document* document) const{
	rapidjson::Value messageValue = Message::json(document);
	auto content = data.json(document);
	messageValue.AddMember("message", content, document->GetAllocator());
	return messageValue;
}
