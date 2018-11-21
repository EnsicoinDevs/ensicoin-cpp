#include "blocks.hpp"
#include "crypto.hpp"
#include "transaction.hpp"

#include <rapidjson/document.h>
#include <vector>
#include <string>
#include <memory>
#include <sstream>

using namespace rapidjson;

void BlockTarget::update(int timeTaken){

}

bool BlockTarget::blockInferior(const Block& target) const {
	return target.compressedHash() < value;
}

std::string BlockHeader::rawStr() const{
	std::ostringstream os;
	os << version;
	for(const auto& flag: blockFlags){
		os << flag;
	}
	os << hashPrevBlock << 
	      hashTransactions << 
	      timestamp << 
	      nonce;
	return os.str();
}

std::string BlockHeader::hash() const{
	return sha256(sha256(rawStr()),true);
}

std::string Block::compressedHash() const{
	auto blockHash = hash();
	int trailingZeroCount = 0;
	int hashLength = blockHash.length();
	while( trailingZeroCount < hashLength && blockHash[trailingZeroCount] == '0')
		++trailingZeroCount;
	// From here on trailingZeroCount count the number of trailling zeros
	int mantissaSize = 6;
	int exponent = (64-trailingZeroCount-mantissaSize) / 2;
	std::string mantissa = blockHash.substr(trailingZeroCount, mantissaSize);
	
	std::stringstream ss;
	ss << std::hex << exponent << mantissa;
	return ss.str();
}

Block::Block(BlockHeader head, std::vector<std::shared_ptr<Transaction> > transactionList) :
	header(head),
	transactions(transactionList) {}

Block::Block() : header({ 0, {}, "", "", 0, 0}), transactions({}) {}

Block::Block(rapidjson::Document* doc){
	auto& headerVal = (*doc)["header"];
	std::vector< std::string > flagVec;
	for(auto& flag : headerVal["flags"].GetArray()){
		flagVec.push_back(flag.GetString());
	}
	BlockHeader head = { 	headerVal["version"].GetInt(), 
				flagVec, 
				headerVal["hashPrevBlock"].GetString(),
				headerVal["hashTransactions"].GetString(),
				headerVal["timestamp"].GetInt(),
				headerVal["nonce"].GetInt() };

	for(auto& tr : (*doc)["transactions"].GetArray()){
		rapidjson::Document trDoc;
		trDoc.SetObject();
		trDoc.CopyFrom(tr, trDoc.GetAllocator());
		
		transactions.push_back(std::make_shared<Transaction>(&trDoc));
	}
}

Document Block::json() const {
	Document document(kObjectType);
	document.AddMember("version", header.version, document.GetAllocator());

	Value flagArray(kArrayType);
	for(const auto& flag: header.blockFlags){
		Value flagValue;
		flagValue.SetString(flag.c_str(), flag.length(), document.GetAllocator()); 
		flagArray.PushBack(flagValue, document.GetAllocator());
	}
	document.AddMember("flags", flagArray, document.GetAllocator());

	Value hashPrev;
	hashPrev.SetString(header.hashPrevBlock.c_str(), header.hashPrevBlock.length(), document.GetAllocator());
	document.AddMember("hashPrevBlock",hashPrev, document.GetAllocator());
	Value hashTransactionValue;
	hashTransactionValue.SetString(header.hashTransactions.c_str(), header.hashTransactions.length(), document.GetAllocator());
	document.AddMember("hashTransactions",hashTransactionValue, document.GetAllocator());

	Value timestampValue(header.timestamp);
	document.AddMember("timestamp", timestampValue, document.GetAllocator());

	Value nonceValue(header.nonce);
	document.AddMember("nonce", nonceValue, document.GetAllocator());

	Value transactionArray(kArrayType);
	for(const auto& transaction : transactions){
		transactionArray.PushBack(transaction->json(), document.GetAllocator());
	}

	document.AddMember("transactions", transactionArray, document.GetAllocator());

	return document;
}

std::string Block::rawStr() const {
	std::ostringstream os;
	os << header.rawStr();
	for(const auto& transaction : transactions){
		os << transaction->rawStr(); 
	}
	return (const std::string) os.str();
}

std::string Block::hash() const{
	return header.hash();
}

bool Block::validate(){
	return true;
}
