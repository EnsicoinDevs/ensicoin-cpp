#include "blocks.hpp"
#include "crypto.hpp"
#include "networkable.hpp"
#include "networkbuffer.hpp"
#include "transaction.hpp"

#include <cryptopp/integer.h>
#include <cryptopp/hex.h>
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

	std::string BlockHeader::byteRepr() const{
		std::ostringstream ss;
		ss << networkable::Uint32(version).byteRepr()
		   << networkable::Var_strArray(blockFlags).byteRepr()
		   << networkable::Hash(hashPrevBlock).byteRepr()
		   << networkable::Hash(merkleRoot).byteRepr()
		   << networkable::Uint64(timestamp).byteRepr()
		   << networkable::Uint32(height).byteRepr()
		   << target.byteRepr()
		   << networkable::Uint64(nonce).byteRepr();
		return ss.str();
	}

	Block::Block(BlockHeader head, std::vector<std::shared_ptr<Transaction> > transactionList) :
		header(head),
		transactions(transactionList) {}


	BlockHeader Block::getHeader() const{
		return header;
	}

	std::string Block::byteRepr() const{
		return header.byteRepr() +
			networkable::Var_Array<Transaction::pointer>(transactions).byteRepr();
	}

	Block::Block(NetworkBuffer* networkBuffer) : 
		header(networkBuffer),
		transactions(networkable::Var_Array<Transaction::pointer>(networkBuffer).getData()) {}

	CryptoPP::Integer Block::integerHash() const {
		std::string binaryHash = hash();
		std::string encoded;
		CryptoPP::StringSource ss(binaryHash, true,
    		new CryptoPP::HexEncoder(
        		new CryptoPP::StringSink(encoded)
    		) // HexEncoder
		); // StringSource

		return CryptoPP::Integer(("0x"+encoded).c_str());
	}

	rapidjson::Document Block::json() const {
		rapidjson::Document document;
		document.SetObject();
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
		return sha256(sha256(header.byteRepr()));
	}

	bool Block::validate(){
		return true;
	}
} // namespace ressources
