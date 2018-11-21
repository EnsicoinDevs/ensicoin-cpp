#include "messages.hpp"
#include "blocks.hpp"

#include <memory>
#include <rapidjson/document.h>

BlockMessage::BlockMessage(std::shared_ptr<Block> blockPtr) : Message("block"), block(blockPtr) {}

BlockMessage::BlockMessage(rapidjson::Document* doc) : Message(doc) {
	auto& blockVal = (*doc)["message"];
	rapidjson::Document blockDoc;
	blockDoc.SetObject();
	blockDoc.CopyFrom(blockVal, blockDoc.GetAllocator());
	block = std::make_shared<Block>(&blockDoc);
}

rapidjson::Value BlockMessage::json(rapidjson::Document* document) const{
	rapidjson::Value messageValue = Message::json(document);
	//rapidjson::Value content = block->json(document);
	
	//messageValue.AddMember("message", content, document->GetAllocator());
	return messageValue;
}
