#include "transaction.hpp"
#include "constants.hpp"
#include "crypto.hpp"

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <sstream>
#include <string>
#include <vector>

using namespace rapidjson;

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
				outputs(initialOutputs) {}

Transaction::Transaction(rapidjson::Document* doc) : version((*doc)["version"].GetInt()){
	for(auto& flag : (*doc)["flags"].GetArray()){
		transactionFlags.push_back(flag.GetString());
	}
	for(auto& input : (*doc)["inputs"].GetArray()){
		InputTransaction inp;
		inp.load(&input);
		inputs.push_back(inp);
	}
	for(auto& output : (*doc)["outputs"].GetArray()){
		OutputTransaction out;
		out.load(&output);
		outputs.push_back(out);
	}
}

int Transaction::getVersion() const{
	return version;
}

std::vector<std::string> Transaction::getFlags() const {
	return transactionFlags;
}


bool Transaction::check(){
	if(inputs.empty() || outputs.empty())
		return false;
	if(str().length() > MAX_BLOCK_SIZE)
		return false;
	for(auto& out : outputs){
		if(out.value < 0)
			return false;
	}
	return true;
}

std::string Transaction::hash() const{
	return sha256(sha256(rawStr()), true);
}

std::string Transaction::str() const{
	rapidjson::Document* d = new rapidjson::Document();
	d->SetObject();

	rapidjson::Value val = this->json(true,d);
	
	rapidjson::StringBuffer docBuffer;
	rapidjson::PrettyWriter<StringBuffer> writer(docBuffer);
	val.Accept(writer);

	return docBuffer.GetString();
}
std::string Transaction::rawStr() const {
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

std::string Transaction::hashWithoutInputs() const{
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

