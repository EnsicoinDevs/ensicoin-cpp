#include "transaction.hpp"

#include <rapidjson/document.h>
#include <sstream>

using rapidjson::Document;
using rapidjson::Value;

std::string InputTransaction::str() const{
	std::ostringstream os;
	os << previousOutput.str();
	auto copyStack = inputStack;
	while(!copyStack.empty()){
		const std::string stackElem(copyStack.top());
		copyStack.pop();
		os << stackElem;
	}
	return os.str();
}
Value InputTransaction::json(Document* document) const {
	Value input(kObjectType);
	input.AddMember("previousOutput", previousOutput.json(document), document->GetAllocator());
	
	Value scriptValue(kArrayType);
	auto copyStack = inputStack;
	while(!copyStack.empty()){
		std::string stackElem(copyStack.top());
		copyStack.pop();
		
		Value stackElemValue;
		stackElemValue.SetString(stackElem.c_str(), stackElem.length(), document->GetAllocator());

		scriptValue.PushBack(stackElemValue, document->GetAllocator());
	}
	input.AddMember("script",scriptValue , document->GetAllocator());

	return input;
}

void InputTransaction::load(rapidjson::Value* val){
	previousOutput.load(&(*val)["previousOutput"]);
	std::stack< std::string > reversed;
	for(auto& stackElem : (*val)["script"].GetArray() ){
		reversed.push(stackElem.GetString());
	}

	while(!reversed.empty()){
		auto elem = reversed.top();
		reversed.pop();
		inputStack.push(elem);
	}

}
