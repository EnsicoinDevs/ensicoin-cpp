#include <string>
#include <ctime>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <iostream>

#include "messages.hpp"

using namespace rapidjson;

Message::Message(std::string messageType) : magic(422021), type(messageType), timestamp(std::time(0)){}

Message::Message(rapidjson::Document* document) : magic((*document)["magic"].GetInt()),
						  type((*document)["type"].GetString()),
						  timestamp((*document)["timestamp"].GetInt()) {}

const std::string Message::getType() const{
	return type;
}

Value Message::json(Document* document) const{
	Value messageValue(kObjectType);
	messageValue.AddMember("magic",magic, document->GetAllocator());

	Value typeStr;
	typeStr.SetString(type.c_str(), type.length(), document->GetAllocator());
	messageValue.AddMember("type",typeStr, document->GetAllocator());

	messageValue.AddMember("timestamp", timestamp, document->GetAllocator());

	return messageValue;
}

const std::string Message::str() const{
	rapidjson::Document* d = new rapidjson::Document();
	d->SetObject();

	rapidjson::Value val = this->json(d);
	
	rapidjson::StringBuffer docBuffer;
	rapidjson::PrettyWriter<StringBuffer> writer(docBuffer);
	val.Accept(writer);

	return docBuffer.GetString();
}

