#include "blocks.hpp"
#include "crypto.hpp"
#include "networkable.hpp"
#include "networkbuffer.cpp"
#include "transaction.hpp"

#include <memory>
#include <rapidjson/document.h>
#include <sstream>
#include <string>
#include <vector>

namespace ressources{
	
	BlockHeader::BlockHeader(NetworkBuffer* networkBuffer) : 
		version(networkable::Uint32(networkBuffer).getValue()),
		blockFlags(networkable::Var_strArray(networkBuffer).getValue()),
		hashPrevBlock(networkable::Hash(networkBuffer).getValue()),
		merkleRoot(networkable::Hash(networkBuffer).getValue()),
		timestamp(networkable::Uint64(networkBuffer).getValue()),
		height(networkable::Uint32(networkBuffer).getValue()),
		target(networkBuffer),
		nonce(networkable::Uint64(networkBuffer).getValue())
	{}

	Block::Block(BlockHeader head, std::vector<std::shared_ptr<Transaction> > transactionList) :
		header(head),
		transactions(transactionList) {}


	Block::Block(NetworkBuffer* networkBuffer) : 
		header(networkBuffer),
		transactions(networkable::Var_Array<Transaction>(networkBuffer).getValue()) {}

	rapidjson::Document Block::json() const {
		rapidjson::Document document(rapidjson::kObjectType);
		document.AddMember("version", header.version, 
				document.GetAllocator());

		rapidjson::Value flagArray(rapidjson::kArrayType);
		for(const auto& flag: header.blockFlags){
			rapidjson::Value flagValue;
			flagValue.SetString(flag.c_str(), 
					flag.length(), document.GetAllocator()); 
			flagArray.PushBack(flagValue, document.GetAllocator());
		}
		document.AddMember("flags", flagArray,
				document.GetAllocator());

		rapidjson::Value hashPrev;
		hashPrev.SetString(	header.hashPrevBlock.c_str(),
				header.hashPrevBlock.length(), 
				document.GetAllocator());
		document.AddMember("hashPrevBlock",hashPrev,
				document.GetAllocator());
		rapidjson::Value hashTransactionValue;
		hashTransactionValue.SetString(
				header.merkleRoot.c_str(), 
				header.merkleRoot.length(), 
				document.GetAllocator());
		document.AddMember("merkleRoot",
				hashTransactionValue, document.GetAllocator());

		rapidjson::Value timestampValue(header.timestamp);
		document.AddMember("timestamp", timestampValue,
				document.GetAllocator());

		rapidjson::Value nonceValue(header.nonce);
		document.AddMember("nonce", nonceValue,
				document.GetAllocator());

		rapidjson::Value transactionArray(rapidjson::kArrayType);
		for(const auto& transaction : transactions){
			transactionArray.PushBack(transaction->json(),
					document.GetAllocator());
		}

		document.AddMember("transactions",
				transactionArray, document.GetAllocator());

		return document;
	}


	std::string Block::hash() const{
		return header.hash();
	}

	bool Block::validate(){
		return true;
	}
} // namespace ressources
