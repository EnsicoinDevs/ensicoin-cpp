#include <string>
#include <vector>
#include <sstream>

#include <rapidjson/document.h>
#include "transaction.hpp"
#include "crypto.hpp"

using namespace rapidjson;

const std::string TransactionIdentifier::str() const{
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

const std::string InputTransaction::str() const{
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

const std::string OutputTransaction::str() const {
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

Transaction::Transaction() : version(42),
			     transactionFlags({}),
			     inputs({}),
			     outputs({}) {}

Transaction::Transaction(	int ver,
				std::vector<std::string> intialFlags,
				std::vector<InputTransaction> initialInputs,
				std::vector<OutputTransaction> initialOutputs) :
				version(ver),
				transactionFlags(intialFlags),
				inputs(initialInputs),
				outputs(initialOutputs){}

const int Transaction::getVersion() const{
	return version;
}

std::vector<std::string> Transaction::getFlags() const {
	return transactionFlags;
}

const std::string Transaction::str() const {
	std::ostringstream os;
	os << version;
	for(const auto& flag: transactionFlags){
		os << flag;
	}
	for(const auto& input : inputs){
		os << input.str();
	}
	for(const auto& output: outputs){
		os << output.str();
	}
	return os.str();
}
Value Transaction::json(bool includeInputs,Document* document) const {
	Value transaction(kObjectType);
	transaction.AddMember("version", version, document->GetAllocator());
	
	Value flagArray(kArrayType);
	for(auto& flag : transactionFlags){
		Value strValue;
		strValue.SetString(flag.c_str(), flag.length(), document->GetAllocator());
		
		flagArray.PushBack(strValue, document->GetAllocator());
	}
	transaction.AddMember("flags",flagArray, document->GetAllocator());
	if(includeInputs){
		Value inputsValue(kArrayType);
		for(auto& input : inputs){
			inputsValue.PushBack(input.json(document), document->GetAllocator());
		}
		transaction.AddMember("inputs",inputsValue,document->GetAllocator());
	}

	Value outputValue(kArrayType);
	for(auto& output : outputs){
		outputValue.PushBack(output.json(document), document->GetAllocator());
	}
	transaction.AddMember("outputs", outputValue, document->GetAllocator());

	return transaction;
}

const std::string Transaction::hashWithoutInputs() const{
	std::ostringstream os;
	os << version;
	for(const auto& flag: transactionFlags){
		os << flag;
	}
	for(const auto& output: outputs){
		os << output.str();
	}
	return sha256(sha256(os.str()), true);
}

const bool Transaction::validate(){
	if(inputs.empty() || outputs.empty()){
		return false;
	}
}
