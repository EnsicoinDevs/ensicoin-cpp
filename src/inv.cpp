#include "messages.hpp"

#include <rapidjson/document.h>
#include <string>
#include <vector>

Inv::Inv(std::string res, std::vector<std::string> hashList) : Message("inv"), ressourceType(res), hashes(hashList) {}

Inv::Inv(rapidjson::Document* doc) : Message(doc), ressourceType((*doc)["message"]["type"].GetString()) {
	auto& hashArray = (*doc)["message"]["hashes"];
	for(unsigned int i = 0; i < hashArray.Size(); i++){
		hashes.push_back(hashArray[i].GetString());
	}
}

rapidjson::Value Inv::json(rapidjson::Document* document) const{
	rapidjson::Value messageValue = Message::json(document);
	rapidjson::Value content(rapidjson::kObjectType);

	rapidjson::Value hashArray(rapidjson::kArrayType);
	for(auto& resHash : hashes){
		rapidjson::Value strVal;
		strVal.SetString(resHash.c_str(), resHash.length(), document->GetAllocator());
		hashArray.PushBack(strVal, document->GetAllocator());
	}
	rapidjson::Value typeVal;
	typeVal.SetString(ressourceType.c_str(), ressourceType.length(), document->GetAllocator());

	content.AddMember("type", typeVal, document->GetAllocator());
	content.AddMember("hashes", hashArray, document->GetAllocator());

	messageValue.AddMember("message", content, document->GetAllocator());
	return messageValue;
}
