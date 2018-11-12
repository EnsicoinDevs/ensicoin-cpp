#include "transaction.hpp"
#include "blocks.hpp"
#include "constants.hpp"
#include "crypto.hpp"
#include "hashmemory.hpp"
#include "mempool.hpp"

#include <algorithm>
#include <memory>
#include <numeric>
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

bool Transaction::hasOutput(int index) const{
	return index < outputs.size();
}

int Transaction::getOutputNumber() const{
	return outputs.size();
}

int Transaction::getOutputValue(int index) const{
	return outputs[index].value;
}

std::vector<TransactionIdentifier> Transaction::getInputsId() const{
	std::vector<TransactionIdentifier> output;
	for(auto& input : inputs)
		output.push_back(input.previousOutput);
	return output;
}

bool Transaction::validate(Mempool* mempool){
	if(!check()) return false;
	if(mempool->exists(hash())) return false;
	if(inputValue(mempool) <= outputValue()) return false;
	if(std::any_of(inputs.begin(), 
		       inputs.end(),
		       [mempool](InputTransaction ip){
				return !mempool->isSpendable(ip.previousOutput);
			})) return false;
	if(!validateScript(mempool)) return false;
	return true;
}

bool Transaction::validateScript(Mempool* mempool) const{
	for(auto& input : inputs){
		auto outputScript = mempool->getOutputScript(input.previousOutput);
		Script script(outputScript.begin(), 
			      outputScript.end(),
			      input.inputStack,
			      mempool->getHashSignature(input.previousOutput));
	}
	return false;
}

bool Transaction::check(){
	if(inputs.empty() || outputs.empty())
		return false;
	if(str().length() > MAX_BLOCK_SIZE)
		return false;
	return !std::any_of(outputs.begin(), outputs.end(), [](OutputTransaction output){return output.value<=0;});
}

int Transaction::outputValue() const{
	auto sum = 0;
	std::accumulate(outputs.begin(),
			outputs.end(),
			sum,
			[](int s, OutputTransaction op){
				return s + op.value;
			});
	return sum;
}

int Transaction::inputValue(Mempool* mempool) const{
	auto sum = 0;
	std::accumulate(inputs.begin(), 
			inputs.end(), 
			sum, 
			[mempool](int s,InputTransaction ip){
				return s + mempool->getInputValue(ip.previousOutput);
			});
	return sum;
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

