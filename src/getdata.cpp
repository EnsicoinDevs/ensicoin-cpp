#include "messages.hpp"

#include <memory>
#include <rapidjson/document.h>

GetData::GetData(Message::messagePointer inv) : Message("getdata"), invData(inv) {}

GetData::GetData(rapidjson::Document* doc) : Message(doc) {
	auto& invVal = (*doc)["message"];
	rapidjson::Document invDoc;
	invDoc.SetObject();
	invDoc.CopyFrom(invVal, invDoc.GetAllocator());
	invData = std::make_shared<Inv>(&invDoc);
}

rapidjson::Value GetData::json(rapidjson::Document* document) const{
	rapidjson::Value messageValue = Message::json(document);
	rapidjson::Value invValue = invData->json(document);
	rapidjson::Value content(rapidjson::kObjectType);
	content.AddMember("inv", invValue, document->GetAllocator());
	
	messageValue.AddMember("messsage", content, document->GetAllocator());

	return messageValue;
}
