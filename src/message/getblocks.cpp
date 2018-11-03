#include "messages.hpp"

#include <string>
#include <vector>
#include <rapidjson/document.h>

GetBlocks::GetBlocks(std::vector< std::string > hashList, std::string stopHashString) : Message("getblocks"), 
											 blockHashes(hashList), 
											 stopHash(stopHashString) {}

GetBlocks::GetBlocks(rapidjson::Document* doc) : Message(doc), stopHash((*doc)["message"]["stopHash"].GetString()) {
	for(auto& hash : (*doc)["message"]["hashes"].GetArray() ){
		blockHashes.push_back(hash.GetString());
	}
}

rapidjson::Value GetBlocks::json(rapidjson::Document* doc) const{
	rapidjson::Value messageValue = Message::json(doc);
	rapidjson::Value stopHashVal;
	rapidjson::Value content(rapidjson::kObjectType);
	rapidjson::Value hashesArray(rapidjson::kArrayType);
	for(auto& hash : blockHashes){
		rapidjson::Value hashVal;
		hashVal.SetString(hash.c_str(), hash.length(), doc->GetAllocator());
		hashesArray.PushBack(hashVal, doc->GetAllocator());
	}
	content.AddMember("hashes", hashesArray, doc->GetAllocator());
	stopHashVal.SetString(stopHash.c_str(), stopHash.length(), doc->GetAllocator());
	content.AddMember("stopHash", stopHashVal, doc->GetAllocator());

	messageValue.AddMember("message", content, doc->GetAllocator());
	return messageValue;
}
