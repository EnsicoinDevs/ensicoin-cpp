#include "transaction.hpp"

#include <rapidjson/document.h>
#include <sstream>

using rapidjson::Document;
using rapidjson::Value;

std::string OutputTransaction::str() const {
	std::ostringstream os;
	os << value;
	for(const std::string& stackElem : scriptInstructions){
		os << stackElem;
	}
	return os.str();
}
Value OutputTransaction::json(Document* document) const {
	Value output(kObjectType);
	output.AddMember("value", value, document->GetAllocator());

	Value script(kArrayType);
	for(auto& scriptElem : scriptInstructions){

		Value strValue;
		strValue.SetString(scriptElem.c_str(), scriptElem.length(), document->GetAllocator());

		script.PushBack(strValue, document->GetAllocator());
	}
	output.AddMember("script", script, document->GetAllocator());

	return output;
}

void OutputTransaction::load(rapidjson::Value* val){
	value = (*val)["value"].GetInt();
	for(auto& scriptElem : (*val)["script"].GetArray() ){
		scriptInstructions.push_back(scriptElem.GetString());
	}
}
