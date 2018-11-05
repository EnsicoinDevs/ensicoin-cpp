#include "transaction.hpp"

#include <rapidjson/document.h>
#include <sstream>

using rapidjson::Document;
using rapidjson::Value;

std::string TransactionIdentifier::str() const{
	std::ostringstream os;
	os << transactionHash;
	os << index;
	return os.str();
}

Value TransactionIdentifier::json(Document* document) const{
	Value tId(kObjectType);
	Value strVal;
	strVal.SetString(transactionHash.c_str(), transactionHash.length(), document->GetAllocator());
	
	tId.AddMember("transactionHash", strVal, document->GetAllocator());
	tId.AddMember("index", index, document->GetAllocator());

	return tId;
}

void TransactionIdentifier::load(rapidjson::Value* val){
	transactionHash = (*val)["transactionHash"].GetString();
	index = (*val)["index"].GetInt();
}
