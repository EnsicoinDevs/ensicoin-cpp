#include "blocks.hpp"
#include "transaction.hpp"


#include <rapidjson/document.h>
#include <vector>
#include <sstream>

using namespace rapidjson;

Block::Block(BlockHeader head, std::vector<Transaction* > transactionList) :
	header(head),
	transactions(transactionList) {}

Block::Block() : header({ 0, {}, "", "", 0, 0}), transactions({}) {}

Value Block::json(Document& document) const {
	Value blockValue(kObjectType);
	blockValue.AddMember("version", header.version, document.GetAllocator());

	Value flagArray(kArrayType);
	for(const auto& flag: header.blockFlags){
		Value flagValue;
		flagValue.SetString(flag.c_str(), flag.length(), document.GetAllocator()); 
		flagArray.PushBack(flagValue, document.GetAllocator());
	}
	blockValue.AddMember("flags", flagArray, document.GetAllocator());

	Value hashPrev;
	hashPrev.SetString(header.hashPrevBlock.c_str(), header.hashPrevBlock.length(), document.GetAllocator());
	blockValue.AddMember("hashPrevBlock",hashPrev, document.GetAllocator());
	Value hashTransactionValue;
	hashTransactionValue.SetString(header.hashTransactions.c_str(), header.hashTransactions.length(), document.GetAllocator());
	blockValue.AddMember("hashTransactions",hashTransactionValue, document.GetAllocator());

	Value timestampValue(header.timestamp);
	blockValue.AddMember("timestamp", timestampValue, document.GetAllocator());

	Value nonceValue(header.nonce);
	blockValue.AddMember("nonce", nonceValue, document.GetAllocator());

	Value transactionArray(kArrayType);
	for(const auto& transaction : transactions){
		transactionArray.PushBack(transaction->json(true, document), document.GetAllocator());
	}

	blockValue.AddMember("transactions", transactionArray, document.GetAllocator());

	return blockValue;
}

const std::string Block::str() const {
	std::ostringstream os;
	os << header.version;
	for(const auto& flag: header.blockFlags){
		os << flag;
	}
	os << header.hashPrevBlock << header.hashTransactions << header.timestamp << header.nonce;
	for(const auto& transaction : transactions){
		os << transaction->str(); 
	}
	return os.str();
}

const bool Block::validate(){
	
}
