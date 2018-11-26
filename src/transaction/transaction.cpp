#include "transaction.hpp"
#include "blocks.hpp"
#include "constants.hpp"
#include "crypto.hpp"
#include "hashmemory.hpp"
#include "mempool.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"

#include <algorithm>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace ressources{

	Transaction::Transaction(	
			int ver,
			std::vector<std::string> intialFlags,
			std::vector<InputTransaction> initialInputs,
			std::vector<OutputTransaction> initialOutputs) :
		version(ver),
		transactionFlags(intialFlags),
		inputs(initialInputs),
		outputs(initialOutputs) {}

	Transaction::Transaction(NetworkBuffer* networkBuffer) {
		
	}

	std::string Transaction::byteRepr() const{
		std::ostringstream ss;
		ss << networkable::Uint32(version).byteRepr()
		   << networkable::Var_uint(transactionFlags\
				   .size()).byteRepr();
		for(auto& flag : transactionFlags){
			ss << networkable::Var_str(flag).byteRepr();
		}
		ss << networkable::Var_uint(inputs.size()).byteRepr();
		for(auto& input : inputs){
			ss << input.byteRepr();
		}
		ss << networkable::Var_uint(outputs.size()).byteRepr();
		for(auto& output : outputs){
			ss << output.byteRepr();
		}
		return ss.str();
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

	unsigned int Transaction::valueOfOutput(unsigned int
			index) const{
		return outputs[index].value;
	}

	std::vector<TransactionIdentifier> Transaction::\
		getInputsId() const{
		std::vector<TransactionIdentifier> output;
		for(auto& input : inputs)
			output.push_back(input.previousOutput);
		return output;
	}

	std::vector<InputTransaction> Transaction::getInputs() const{
		return inputs;
	}

	Script Transaction::getScriptOfOutput
		(int index) const{
		return outputs[index].script;
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
		return sha256(sha256(byteRepr()));
	}

	std::string Transaction::signingHash() const{
		return "";
		/// \todo implement signingHash
	}

	rapidjson::Document Transaction::json() const {
		rapidjson::Document document(rapidjson::kObjectType);
		document.AddMember("version", version, document.GetAllocator());

		rapidjson::Value flagArray(rapidjson::kArrayType);
		for(auto& flag : transactionFlags){
			rapidjson::Value strValue;
			strValue.SetString(flag.c_str(), flag.length(),
					document.GetAllocator());

			flagArray.PushBack(strValue, document.GetAllocator());
		}
		document.AddMember("flags",flagArray,
				document.GetAllocator());
		rapidjson::Value inputsValue(rapidjson::kArrayType);
		for(auto& input : inputs){
			inputsValue.PushBack(input.json(&document),
					document.GetAllocator());
		}
		document.AddMember("inputs",inputsValue,
				document.GetAllocator());


		rapidjson::Value outputValue(rapidjson::kArrayType);
		for(auto& output : outputs){
			outputValue.PushBack(output.json(&document),
					document.GetAllocator());
		}
		document.AddMember("outputs", outputValue,
				document.GetAllocator());

		return document;
	}

} // namespace ressources
