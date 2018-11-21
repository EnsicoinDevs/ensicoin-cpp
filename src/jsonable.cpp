#include "jsonable.hpp"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <string>

std::string JSONAble::str() const{
	auto doc = this->json();
	rapidjson::StringBuffer docBuffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(docBuffer);
	doc.Accept(writer);

	return docBuffer.GetString();
}

JSONAble::~JSONAble() { }
