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

bool Transaction::hasOutput(unsigned int index) const{
	return index < outputs.size();
}

unsigned int Transaction::outputCount() const{
	return outputs.size();
}

unsigned int Transaction::valueOfOutput(unsigned int index) const{
	return outputs[index].value;
}

std::vector<TransactionIdentifier> Transaction::getInputsId() const{
	std::vector<TransactionIdentifier> output;
	for(auto& input : inputs)
		output.push_back(input.previousOutput);
	return output;
}

std::vector<InputTransaction> Transaction::getInputs() const{
	return inputs;
}

std::vector<std::string> Transaction::getScriptOfOutput(int index) const{
	return outputs[index].scriptInstructions;
}


bool Transaction::check(){
	if(inputs.empty() || outputs.empty())
		return false;
	if(str().length() > MAX_BLOCK_SIZE)
		return false;
	return !std::any_of(outputs.begin(), outputs.end(), [](OutputTransaction output){return output.value<=0;});
}

unsigned int Transaction::outputValue() const{
	auto sum = 0;
	std::accumulate(outputs.begin(),
			outputs.end(),
			sum,
			[](int s, OutputTransaction op){
				return s + op.value;
			});
	return sum;
}

std::string Transaction::hash() const{
	return sha256(sha256(rawStr()), true);
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
Document Transaction::json() const {
	Document document(kObjectType);
	document.AddMember("version", version, document.GetAllocator());
	
	Value flagArray(kArrayType);
	for(auto& flag : transactionFlags){
		Value strValue;
		strValue.SetString(flag.c_str(), flag.length(), document.GetAllocator());
		
		flagArray.PushBack(strValue, document.GetAllocator());
	}
	document.AddMember("flags",flagArray, document.GetAllocator());
	Value inputsValue(kArrayType);
	for(auto& input : inputs){
		inputsValue.PushBack(input.json(&document), document.GetAllocator());
	}
	document.AddMember("inputs",inputsValue,document.GetAllocator());
	

	Value outputValue(kArrayType);
	for(auto& output : outputs){
		outputValue.PushBack(output.json(&document), document.GetAllocator());
	}
	document.AddMember("outputs", outputValue, document.GetAllocator());

	return document;
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

